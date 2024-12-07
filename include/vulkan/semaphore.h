#ifndef PHOENIX_VULKAN_SEMAPHORE_H
#define PHOENIX_VULKAN_SEMAPHORE_H

#include "util/error.h"
#include <vulkan/vulkan_core.h>

Error vulkan_semaphore_create(VkSemaphore *semaphore, VkDevice device);

#endif
