#ifndef PHOENIX_VULKAN_PRESENT_MODES_H
#define PHOENIX_VULKAN_PRESENT_MODES_H

#include "util/error.h"
#include <vulkan/vulkan_core.h>

typedef struct VulkanPresentModes {
  VkPresentModeKHR *handles;
  unsigned length;
} VulkanPresentModes;

Error vulkan_present_modes_create(VulkanPresentModes *modes, VkPhysicalDevice device, VkSurfaceKHR surface);
void vulkan_present_modes_destroy(VulkanPresentModes *modes);

#endif
