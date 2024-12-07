#include "vulkan/framebuffers.h"
#include "util/error.h"
#include "vulkan/image_views.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

Error vulkan_framebuffers_create(VulkanFramebuffers *framebuffers, VkDevice device, VkRenderPass render_pass, const VkExtent2D *extent, const VulkanImageViews *views) {
  if(!framebuffers || !views) return NULL_HANDLE_ERROR;

  framebuffers->length = views->length;

  void *tmp = realloc(framebuffers->handles, framebuffers->length * sizeof(VkFramebuffer));
  if(!tmp) return ALLOCATION_ERROR;
  framebuffers->handles = tmp;

  VkFramebufferCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .renderPass = render_pass,
    .attachmentCount = 1,
    .pAttachments = NULL, // Will be set later
    .width = extent->width,
    .height = extent->height,
    .layers = 1,
  };

  for(unsigned index = 0; index < framebuffers->length; ++index) {
    info.pAttachments = views->handles + index;

    if(vkCreateFramebuffer(device, &info, NULL, framebuffers->handles + index) != VK_SUCCESS)
      return VULKAN_FRAMEBUFFER_CREATE_ERROR;
  }

  return SUCCESS;
}

void vulkan_framebuffers_destroy(VulkanFramebuffers *framebuffers, VkDevice device) {
  if(!framebuffers) return;

  for(unsigned index = 0; index < framebuffers->length; ++index)
    if(framebuffers->handles[index])
      vkDestroyFramebuffer(device, framebuffers->handles[index], NULL);

  free(framebuffers->handles);
  framebuffers->handles = 0x0;
}
