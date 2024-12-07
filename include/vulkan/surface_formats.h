#ifndef PHOENIX_VULKAN_SURFACE_FORMATS_H
#define PHOENIX_VULKAN_SURFACE_FORMATS_H

#include "util/error.h"
#include <vulkan/vulkan_core.h>

typedef struct VulkanSurfaceFormats {
  VkSurfaceFormatKHR *handles;
  unsigned length;
} VulkanSurfaceFormats;

Error vulkan_surface_formats_create(VulkanSurfaceFormats *formats, VkPhysicalDevice device, VkSurfaceKHR surface);
void vulkan_surface_formats_destroy(VulkanSurfaceFormats *formats);

#endif
