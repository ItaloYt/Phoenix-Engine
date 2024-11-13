#include "renderer/vulkan/framebuffers.h"
#include "renderer/vulkan/device.h"
#include "renderer/vulkan/image_views.h"
#include "renderer/vulkan/render_pass.h"
#include "renderer/vulkan/swapchain.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

struct Framebuffers {
  RenderPass render_pass;
  ImageViews views;
  Device device;
  VkFramebuffer *handles;
  unsigned length;
};

Error framebuffers_create(Framebuffers *framebuffers, RenderPass render_pass, ImageViews views) {
  if(!framebuffers || !render_pass || !views) return NULL_HANDLE_ERROR;

  const VkRenderPass vk_render_pass = render_pass_get_handle(render_pass);
  const Swapchain swapchain = render_pass_get_swapchain(render_pass);
  const VkExtent2D *extent = swapchain_get_extent(swapchain);
  const Device device = render_pass_get_device(render_pass);
  const VkDevice vk_device = device_get_handle(device);
  const VkImageView *vk_views = image_views_get_handles(views);

  *framebuffers = malloc(sizeof(struct Framebuffers));
  if(!*framebuffers) return ALLOCATION_ERROR;

  (*framebuffers)->render_pass = render_pass;
  (*framebuffers)->views = views;
  (*framebuffers)->device = device;
  (*framebuffers)->length = image_views_get_length(views);
  (*framebuffers)->handles = malloc((*framebuffers)->length * sizeof(VkFramebuffer));
  if(!(*framebuffers)->handles) return ALLOCATION_ERROR;

  VkFramebufferCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .renderPass = vk_render_pass,
    .attachmentCount = 1,
    .pAttachments = NULL, // Will be set later
    .width = extent->width,
    .height = extent->height,
    .layers = 1,
  };

  for(unsigned index = 0; index < (*framebuffers)->length; ++index) {
    info.pAttachments = vk_views + index;

    if(vkCreateFramebuffer(vk_device, &info, NULL, (*framebuffers)->handles + index) != VK_SUCCESS)
      return FRAMEBUFFER_CREATE_ERROR;
  }

  return SUCCESS;
}

void framebuffers_destroy(Framebuffers framebuffers) {
  if(!framebuffers) return;

  const VkDevice vk_device = device_get_handle(framebuffers->device);

  for(unsigned index = 0; index < framebuffers->length; ++index)
    vkDestroyFramebuffer(vk_device, framebuffers->handles[index], NULL);

  free(framebuffers->handles);
  free(framebuffers);
}

RenderPass framebuffers_get_render_pass(Framebuffers framebuffers) { return (!framebuffers ? NULL : framebuffers->render_pass); }
ImageViews framebuffers_get_image_views(Framebuffers framebuffers) { return (!framebuffers ? NULL : framebuffers->views); }
Device framebuffers_get_device(Framebuffers framebuffers) { return (!framebuffers ? NULL : framebuffers->device); }
const VkFramebuffer *framebuffers_get_handles(Framebuffers framebuffers) { return (!framebuffers ? NULL : framebuffers->handles); }
unsigned framebuffers_get_length(Framebuffers framebuffers) { return (!framebuffers ? 0 : framebuffers->length); }
