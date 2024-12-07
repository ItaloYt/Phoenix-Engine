#ifndef PHOENIX_VULKAN_IMAGE_VIEWS_H
#define PHOENIX_VULKAN_IMAGE_VIEWS_H

#include "util/error.h"
#include "vulkan/images.h"
#include <vulkan/vulkan_core.h>

typedef struct VulkanImageViews {
  VkImageView *handles;
  unsigned length;
} VulkanImageViews;

Error vulkan_image_views_create(VulkanImageViews *views, VkDevice device, VkFormat format, const VulkanImages *images);
void vulkan_image_views_destroy(VulkanImageViews *views, VkDevice device);

#endif
