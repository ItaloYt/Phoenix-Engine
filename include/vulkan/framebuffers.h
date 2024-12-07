#ifndef PHOENIX_VULKAN_FRAMEBUFFERS_H
#define PHOENIX_VULKAN_FRAMEBUFFERS_H

#include "util/error.h"
#include "vulkan/image_views.h"
#include <vulkan/vulkan_core.h>

typedef struct VulkanFramebuffers {
  VkFramebuffer *handles;
  unsigned length;
} VulkanFramebuffers;

Error vulkan_framebuffers_create(VulkanFramebuffers *framebuffers, VkDevice device, VkRenderPass render_pass, const VkExtent2D *extent, const VulkanImageViews *views);
void vulkan_framebuffers_destroy(VulkanFramebuffers *framebuffers, VkDevice device);

#endif
