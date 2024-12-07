#ifndef PHOENIX_VULKAN_DEVICE_H
#define PHOENIX_VULKAN_DEVICE_H

#include "util/error.h"
#include <vulkan/vulkan_core.h>

Error vulkan_device_create(VkDevice *device, VkPhysicalDevice physical, unsigned *indices, unsigned *count, float **priorities, unsigned length);

#endif
