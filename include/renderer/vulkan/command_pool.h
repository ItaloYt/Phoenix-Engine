#ifndef PHOENIX_RENDERER_VULKAN_COMMAND_POOL_H
#define PHOENIX_RENDERER_VULKAN_COMMAND_POOL_H

#include "renderer/vulkan/device.h"
#include "util/error.h"
#include <vulkan/vulkan_core.h>

typedef struct CommandPool *CommandPool;

Error command_pool_create(CommandPool *pool, Device device, unsigned family_index);
void command_pool_destroy(CommandPool pool);

Device command_pool_get_device(CommandPool pool);
VkCommandPool command_pool_get_handle(CommandPool pool);

#endif
