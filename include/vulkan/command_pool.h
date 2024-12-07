#ifndef PHOENIX_VULKAN_COMMAND_POOL_H
#define PHOENIX_VULKAN_COMMAND_POOL_H

#include "util/error.h"
#include <vulkan/vulkan_core.h>

Error vulkan_command_pool_create(VkCommandPool *pool, VkDevice device, unsigned index);

#endif
