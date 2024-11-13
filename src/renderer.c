#include "renderer.h"
#include "renderer/vulkan/command_buffer.h"
#include "renderer/vulkan/command_pool.h"
#include "renderer/vulkan/device.h"
#include "renderer/vulkan/fence.h"
#include "renderer/vulkan/framebuffers.h"
#include "renderer/vulkan/image_views.h"
#include "renderer/vulkan/images.h"
#include "renderer/vulkan/instance.h"
#include "renderer/vulkan/physical_devices.h"
#include "renderer/vulkan/pipeline.h"
#include "renderer/vulkan/pipeline_layout.h"
#include "renderer/vulkan/queue.h"
#include "renderer/vulkan/render_pass.h"
#include "renderer/vulkan/semaphore.h"
#include "renderer/vulkan/shader_module.h"
#include "renderer/vulkan/surface.h"
#include "renderer/vulkan/swapchain.h"
#include "util/error.h"
#include "util/resource.h"
#include "window.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

struct Renderer {
  Window window;
  Instance instance;
  Surface surface;
  PhysicalDevices physicals;
  Device device;
  Queue graphic;
  Queue present;
  Swapchain swapchain;
  Images images;
  ImageViews views;
  RenderPass render_pass;
  ShaderModule vertex_shader;
  ShaderModule fragment_shader;
  PipelineLayout layout;
  Pipeline pipeline;
  Framebuffers framebuffers;
  CommandPool pool;
  CommandBuffer buffer; // Graphic Command Buffer
  Semaphore render_signal;
  Fence frame_signal;
};

Error renderer_create(Renderer *renderer, Window window, const char *name, unsigned version) {
  if(!renderer || !window) return NULL_HANDLE_ERROR;

  *renderer = malloc(sizeof(struct Renderer));
  if(!*renderer) return ALLOCATION_ERROR;

  (*renderer)->window = window;

  if(instance_create(&(*renderer)->instance, window, name, version) != SUCCESS)
    return INSTANCE_CREATE_ERROR;

  if(surface_create(&(*renderer)->surface, (*renderer)->instance) != SUCCESS)
    return SURFACE_CREATE_ERROR;

  if(physical_devices_create(&(*renderer)->physicals, (*renderer)->instance) != SUCCESS)
    return RENDERER_PHYSICALS_CREATE_ERROR;

  const VkPhysicalDevice *vk_physical_devices = physical_devices_get_handles((*renderer)->physicals);
  const unsigned vk_physical_devices_length = physical_devices_get_length((*renderer)->physicals);
  VkPhysicalDevice vk_physical_device;

  if(select_physical_device(vk_physical_devices, vk_physical_devices_length, &vk_physical_device) != SUCCESS)
    return RENDERER_SELECT_PHYSICAL_ERROR;

  if(device_create(&(*renderer)->device, (*renderer)->surface, vk_physical_device) != SUCCESS)
    return DEVICE_CREATE_ERROR;

  const unsigned graphic_family_index = device_get_graphic_family_index((*renderer)->device);
  const unsigned present_family_index = device_get_present_family_index((*renderer)->device);

  if(queue_create(&(*renderer)->graphic, (*renderer)->device, graphic_family_index) != SUCCESS)
    return RENDERER_QUEUE_CREATE_ERROR;

  if(queue_create(&(*renderer)->present, (*renderer)->device, present_family_index) != SUCCESS)
    return RENDERER_QUEUE_CREATE_ERROR;

  const VkSurfaceFormatKHR *formats = device_get_surface_formats((*renderer)->device);
  const unsigned formats_length = device_get_surface_formats_length((*renderer)->device);

  const VkPresentModeKHR *modes = device_get_surface_present_modes((*renderer)->device);
  const unsigned modes_length = device_get_surface_present_modes_length((*renderer)->device);

  VkSurfaceFormatKHR format;
  VkPresentModeKHR mode;

  if(select_surface_format(formats, formats_length, &format) != SUCCESS)
    return RENDERER_SELECT_SURFACE_FORMAT_ERROR;

  if(select_surface_present_mode(modes, modes_length, &mode) != SUCCESS)
    return RENDERER_SELECT_SURFACE_PRESENT_MODE_ERROR;

  // TODO: Search for values in swapchain
  if(swapchain_create(&(*renderer)->swapchain, (*renderer)->device, &format, mode) != SUCCESS)
    return SWAPCHAIN_CREATE_ERROR;

  if(images_create(&(*renderer)->images, (*renderer)->swapchain) != SUCCESS)
    return RENDERER_IMAGES_CREATE_ERROR;

  if(image_views_create(&(*renderer)->views, (*renderer)->images) != SUCCESS)
    return IMAGE_VIEWS_CREATE_ERROR;

  if(render_pass_create(&(*renderer)->render_pass, (*renderer)->swapchain) != SUCCESS)
    return RENDER_PASS_CREATE_ERROR;

  // Vertex Shader
  const void *shader_data = resource_get_handle(resources[0]);
  unsigned shader_size = resource_get_size(resources[0]);

  if(shader_module_create(&(*renderer)->vertex_shader, (*renderer)->device, shader_data, shader_size) != SUCCESS)
    return SHADER_MODULE_CREATE_ERROR;

  shader_data = resource_get_handle(resources[1]);
  shader_size = resource_get_size(resources[1]);

  if(shader_module_create(&(*renderer)->fragment_shader, (*renderer)->device, shader_data, shader_size) != SUCCESS)
    return SHADER_MODULE_CREATE_ERROR;

  if(pipeline_layout_create(&(*renderer)->layout, (*renderer)->device) != SUCCESS)
    return PIPELINE_LAYOUT_CREATE_ERROR;

  if(pipeline_create(&(*renderer)->pipeline, (*renderer)->layout, (*renderer)->render_pass, (*renderer)->vertex_shader, (*renderer)->fragment_shader) != SUCCESS)
    return PIPELINE_CREATE_ERROR;

  // FRAMEBUFFER_CREATE_ERROR is an error caused by the error in creating a SINGLE framebuffer, not a general error
  if(framebuffers_create(&(*renderer)->framebuffers, (*renderer)->render_pass, (*renderer)->views) != SUCCESS)
    return RENDERER_FRAMEBUFFERS_CREATE_ERROR;

  if(command_pool_create(&(*renderer)->pool, (*renderer)->device, graphic_family_index) != SUCCESS)
    return COMMAND_POOL_CREATE_ERROR;

  if(command_buffer_create(&(*renderer)->buffer, (*renderer)->pool) != SUCCESS)
    return COMMAND_BUFFER_CREATE_ERROR;

  if(semaphore_create(&(*renderer)->render_signal, (*renderer)->device) != SUCCESS)
    return SEMAPHORE_CREATE_ERROR;

  if(fence_create(&(*renderer)->frame_signal, (*renderer)->device) != SUCCESS)
    return FENCE_CREATE_ERROR;

  return SUCCESS;
}

Error renderer_draw(Renderer renderer) {
  if(!renderer) return NULL_HANDLE_ERROR;

  const VkCommandBuffer vk_buffer = command_buffer_get_handle(renderer->buffer);
  const VkPipeline vk_pipeline = pipeline_get_handle(renderer->pipeline);
  const Semaphore image_signal = swapchain_get_image_signal(renderer->swapchain);

  if(fences_wait((Fence[]){ renderer->frame_signal }, 1) != SUCCESS)
    return FENCES_WAIT_ERROR;

  if(fences_reset((Fence[]){ renderer->frame_signal }, 1) != SUCCESS)
    return FENCES_RESET_ERROR;

  if(swapchain_acquire_image(renderer->swapchain, NULL) != SUCCESS)
    return SWAPCHAIN_ACQUIRE_IMAGE_ERROR;

  unsigned image_index = swapchain_get_image_index(renderer->swapchain);

  // Record Command Buffer
  if(command_buffer_begin(renderer->buffer, renderer->render_pass, renderer->framebuffers, image_index) != SUCCESS)
    return COMMAND_BUFFER_BEGIN_ERROR;

  vkCmdBindPipeline(vk_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vk_pipeline);
  vkCmdDraw(vk_buffer, 3, 1, 0, 0);

  if(command_buffer_end(renderer->buffer) != SUCCESS)
    return COMMAND_BUFFER_END_ERROR;

  // Submit
  if(queue_submit(
       renderer->graphic, renderer->buffer, (Semaphore[]){ image_signal },
       (VkPipelineStageFlags[]){ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT },
       1, (Semaphore[]){ renderer->render_signal }, 1, renderer->frame_signal
     ) != SUCCESS)
    return QUEUE_SUBMIT_ERROR;

  if(queue_present(renderer->present, renderer->swapchain, (Semaphore[]){ renderer->render_signal }, 1) != SUCCESS)
    return QUEUE_PRESENT_ERROR;

  return SUCCESS;
}

void renderer_destroy(Renderer renderer) {
  if(!renderer) return;

  fence_destroy(renderer->frame_signal);
  semaphore_destroy(renderer->render_signal);
  command_buffer_destroy(renderer->buffer);
  command_pool_destroy(renderer->pool);
  framebuffers_destroy(renderer->framebuffers);
  pipeline_destroy(renderer->pipeline);
  pipeline_layout_destroy(renderer->layout);
  shader_module_destroy(renderer->fragment_shader);
  shader_module_destroy(renderer->vertex_shader);
  render_pass_destroy(renderer->render_pass);
  image_views_destroy(renderer->views);
  images_destroy(renderer->images);
  swapchain_destroy(renderer->swapchain);
  queue_destroy(renderer->present);
  queue_destroy(renderer->graphic);
  device_destroy(renderer->device);
  physical_devices_destroy(renderer->physicals);
  surface_destroy(renderer->surface);
  instance_destroy(renderer->instance);
}

#pragma weak select_physical_device
Error select_physical_device(const VkPhysicalDevice *devices, unsigned length, VkPhysicalDevice *device) {
  *device = devices[0];

  return SUCCESS;
}

#pragma weak select_surface_format
Error select_surface_format(const VkSurfaceFormatKHR *formats, unsigned length, VkSurfaceFormatKHR *format) {
  for(unsigned index = 0; index < length; ++index) {
    if(formats[index].format == VK_FORMAT_B8G8R8A8_SRGB && formats[index].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      *format = formats[index];
      return SUCCESS;
    }
  }

  *format = formats[0];

  return SUCCESS;
}

#pragma weak select_surface_present_mode
Error select_surface_present_mode(const VkPresentModeKHR *modes, unsigned length, VkPresentModeKHR *mode) {
  for(unsigned index = 0; index < length; ++index) {
    if(modes[index] == VK_PRESENT_MODE_MAILBOX_KHR) {
      *mode = VK_PRESENT_MODE_MAILBOX_KHR;
      return SUCCESS;
    }
  }

  *mode = VK_PRESENT_MODE_FIFO_KHR;

  return SUCCESS;
}

Window renderer_get_window(Renderer renderer) { return (!renderer ? NULL : renderer->window); }
Instance renderer_get_instance(Renderer renderer) { return (!renderer ? NULL : renderer->instance); }
Surface renderer_get_surface(Renderer renderer) { return (!renderer ? NULL : renderer->surface); }
PhysicalDevices renderer_get_physicals(Renderer renderer) { return (!renderer ? NULL : renderer->physicals); }
Device renderer_get_device(Renderer renderer) { return (!renderer ? NULL : renderer->device); }
Queue renderer_get_graphic(Renderer renderer) { return (!renderer ? NULL : renderer->graphic); }
Queue renderer_get_present(Renderer renderer) { return (!renderer ? NULL : renderer->present); }
Swapchain renderer_get_swapchain(Renderer renderer) { return (!renderer ? NULL : renderer->swapchain); }
Images renderer_get_images(Renderer renderer) { return (!renderer ? NULL : renderer->images); }
ImageViews renderer_get_views(Renderer renderer) { return (!renderer ? NULL : renderer->views); }
RenderPass renderer_get_render_pass(Renderer renderer) { return (!renderer ? NULL : renderer->render_pass); }
ShaderModule renderer_get_vertex_shader(Renderer renderer) { return (!renderer ? NULL : renderer->vertex_shader); }
ShaderModule renderer_get_fragment_shader(Renderer renderer) { return (!renderer ? NULL : renderer->fragment_shader); }
PipelineLayout renderer_get_layout(Renderer renderer) { return (!renderer ? NULL : renderer->layout); }
Pipeline renderer_get_pipeline(Renderer renderer) { return (!renderer ? NULL : renderer->pipeline); }
Framebuffers renderer_get_framebuffers(Renderer renderer) { return (!renderer ? NULL : renderer->framebuffers); }
CommandPool renderer_get_pool(Renderer renderer) { return (!renderer ? NULL : renderer->pool); }
CommandBuffer renderer_get_buffer(Renderer renderer) { return (!renderer ? NULL : renderer->buffer); }
Semaphore renderer_get_render_signal(Renderer renderer) { return (!renderer ? NULL : renderer->render_signal); }
Fence renderer_get_frame_signal(Renderer renderer) { return (!renderer ? NULL : renderer->frame_signal); }
