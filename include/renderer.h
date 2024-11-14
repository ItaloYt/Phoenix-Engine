#ifndef PHOENIX_RENDERER_H
#define PHOENIX_RENDERER_H

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
#include "window.h"
#include <vulkan/vulkan_core.h>

typedef struct Renderer *Renderer;

extern const unsigned renderer_frames_maximum;

Error renderer_create(Renderer *renderer, Window window, const char *name, unsigned version);
Error renderer_draw(Renderer renderer);
void renderer_resize(Renderer renderer);
void renderer_destroy(Renderer renderer);

Error select_physical_device(const VkPhysicalDevice *devices, unsigned length, VkPhysicalDevice *device);
Error select_surface_format(const VkSurfaceFormatKHR *formats, unsigned length, VkSurfaceFormatKHR *format);
Error select_surface_present_mode(const VkPresentModeKHR *modes, unsigned length, VkPresentModeKHR *mode);

Window renderer_get_window(Renderer renderer);
Instance renderer_get_instance(Renderer renderer);
Surface renderer_get_surface(Renderer renderer);
PhysicalDevices renderer_get_physicals(Renderer renderer);
Device renderer_get_device(Renderer renderer);
Queue renderer_get_graphic(Renderer renderer);
Queue renderer_get_present(Renderer renderer);
Swapchain renderer_get_swapchain(Renderer renderer);
Images renderer_get_images(Renderer renderer);
ImageViews renderer_get_views(Renderer renderer);
RenderPass renderer_get_render_pass(Renderer renderer);
ShaderModule renderer_get_vertex_shader(Renderer renderer);
ShaderModule renderer_get_fragment_shader(Renderer renderer);
PipelineLayout renderer_get_layout(Renderer renderer);
Pipeline renderer_get_pipeline(Renderer renderer);
Framebuffers renderer_get_framebuffers(Renderer renderer);
CommandPool renderer_get_pool(Renderer renderer);
CommandBuffer renderer_get_buffer(Renderer renderer);
Semaphore renderer_get_render_signal(Renderer renderer);
Fence renderer_get_frame_signal(Renderer renderer);

#endif
