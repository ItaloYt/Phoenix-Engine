#ifndef PHOENIX_VULKAN_FENCE_H
#define PHOENIX_VULKAN_FENCE_H

#include "util/error.h"
#include <vulkan/vulkan_core.h>

Error vulkan_fence_create(VkFence *fence, VkDevice device);

#endif
