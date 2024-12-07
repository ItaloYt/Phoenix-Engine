#ifndef PHOENIX_VULKAN_DEVICE_MEMORY_H
#define PHOENIX_VULKAN_DEVICE_MEMORY_H

#include "util/error.h"
#include <vulkan/vulkan_core.h>

Error vulkan_device_memory_create(VkDeviceMemory *memory, VkDevice device, unsigned size, unsigned index);
Error vulkan_device_memory_find_memory(unsigned *index, VkPhysicalDevice physical, unsigned filter, VkMemoryPropertyFlags flags, VkMemoryPropertyFlags exclude);

#endif
