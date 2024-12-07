#ifndef PHOENIX_VULKAN_PHYSICAL_DEVICES_H
#define PHOENIX_VULKAN_PHYSICAL_DEVICES_H

#include "util/error.h"
#include <vulkan/vulkan_core.h>

typedef struct VulkanPhysicalDevices {
  VkPhysicalDevice *handles;
  unsigned length;
} VulkanPhysicalDevices;

Error vulkan_physical_devices_create(VulkanPhysicalDevices *devices, VkInstance instance);
void vulkan_physical_devices_destroy(VulkanPhysicalDevices *devices);

#endif
