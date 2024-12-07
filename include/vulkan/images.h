#ifndef PHOENIX_VULKAN_IMAGES_H
#define PHOENIX_VULKAN_IMAGES_H

#include "util/error.h"
#include <vulkan/vulkan_core.h>

typedef struct VulkanImages {
  VkImage *handles;
  unsigned length;
} VulkanImages;

Error vulkan_images_create(VulkanImages *images, VkDevice device, VkSwapchainKHR swapchain);
void vulkan_images_destroy(VulkanImages *images);

#endif
