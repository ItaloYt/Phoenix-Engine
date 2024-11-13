#ifndef PHOENIX_RENDERER_VULKAN_COMMAND_BUFFER_H
#define PHOENIX_RENDERER_VULKAN_COMMAND_BUFFER_H

#include "renderer/vulkan/command_pool.h"
#include "renderer/vulkan/device.h"
#include "util/error.h"
#include <vulkan/vulkan_core.h>

typedef struct CommandBuffer *CommandBuffer;

Error command_buffer_create(CommandBuffer *buffer, CommandPool pool);
Error command_buffer_begin(CommandBuffer buffer);
void command_buffer_destroy(CommandBuffer buffer);

CommandPool command_buffer_get_command_pool(CommandBuffer buffer);
Device command_buffer_get_device(CommandBuffer buffer);
VkCommandBuffer command_buffer_get_handle(CommandBuffer buffer);

#endif
