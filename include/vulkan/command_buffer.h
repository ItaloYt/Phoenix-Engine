#ifndef PHOENIX_VULKAN_COMMAND_BUFFER_H
#define PHOENIX_VULKAN_COMMAND_BUFFER_H

#include "util/error.h"
#include <vulkan/vulkan_core.h>

Error vulkan_command_buffers_create(VkCommandBuffer *buffers, VkDevice device, VkCommandPool pool, unsigned length);
Error vulkan_command_buffer_begin(VkCommandBuffer buffer, VkCommandBufferUsageFlags flags);
void vulkan_command_buffer_begin_render_pass(VkCommandBuffer buffer, VkRenderPass render_pass, VkFramebuffer framebuffer, const VkExtent2D *extent);

#endif
