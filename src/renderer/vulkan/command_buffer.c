#include "renderer/vulkan/command_buffer.h"
#include "renderer/vulkan/command_pool.h"
#include "renderer/vulkan/device.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

struct CommandBuffer {
  CommandPool pool;
  Device device;
  VkCommandBuffer handle;
};

Error command_buffer_create(CommandBuffer *buffer, CommandPool pool) {
  if(!buffer || !pool) return NULL_HANDLE_ERROR;

  const VkCommandPool vk_pool = command_pool_get_handle(pool);
  const Device device = command_pool_get_device(pool);
  const VkDevice vk_device = device_get_handle(device);

  *buffer = malloc(sizeof(struct CommandBuffer));
  if(!*buffer) return ALLOCATION_ERROR;

  (*buffer)->pool = pool;
  (*buffer)->device = device;

  VkCommandBufferAllocateInfo info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    .pNext = NULL,
    .commandPool = vk_pool,
    .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    .commandBufferCount = 1,
  };

  if(vkAllocateCommandBuffers(vk_device, &info, &(*buffer)->handle) != VK_SUCCESS)
    return COMMAND_BUFFER_CREATE_ERROR;

  return SUCCESS;
}

Error command_buffer_begin(CommandBuffer buffer) {
  if(!buffer) return NULL_HANDLE_ERROR;

  VkCommandBufferBeginInfo info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .pNext = NULL,
    .flags = 0,
    .pInheritanceInfo = NULL,
  };

  if(vkBeginCommandBuffer(buffer->handle, &info) != VK_SUCCESS)
    return COMMAND_BUFFER_BEGIN_ERROR;

  return SUCCESS;
}

// TODO: Is it better to create inside the CommandPool?
void command_buffer_destroy(CommandBuffer buffer) {
  if(!buffer) return;

  free(buffer);
}

CommandPool command_buffer_get_command_pool(CommandBuffer buffer) { return (!buffer ? NULL : buffer->pool); }
Device command_buffer_get_device(CommandBuffer buffer) { return (!buffer ? NULL : buffer->device); }
VkCommandBuffer command_buffer_get_handle(CommandBuffer buffer) { return (!buffer ? NULL : buffer->handle); }
