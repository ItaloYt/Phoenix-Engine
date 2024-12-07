#include "vulkan/raw_buffer.h"
#include "util/error.h"
#include <string.h>
#include <vulkan/vulkan_core.h>

Error vulkan_raw_buffer_create(VkBuffer *buffer, VkDevice device, unsigned size, VkBufferUsageFlags usage, VkSharingMode sharing, unsigned *queues, unsigned length) {
  VkBufferCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .size = size,
    .usage = usage,
    .sharingMode = sharing,
    .queueFamilyIndexCount = length,
    .pQueueFamilyIndices = queues,
  };

  if(vkCreateBuffer(device, &info, NULL, buffer) != VK_SUCCESS)
    return VULKAN_RAW_BUFFER_CREATE_ERROR;

  return SUCCESS;
}
