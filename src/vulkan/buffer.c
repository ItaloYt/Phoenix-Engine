#include "vulkan/buffer.h"
#include "util/error.h"
#include "vulkan/command_buffer.h"
#include "vulkan/device_memory.h"
#include "vulkan/queue.h"
#include "vulkan/raw_buffer.h"
#include <string.h>
#include <vulkan/vulkan_core.h>
// Error vulkan_raw_buffer_create(
//   VkBuffer *buffer, VkDevice device, unsigned size,
//   VkBufferUsageFlags usage, VkSharingMode sharing,
//   unsigned *queues, unsigned length

// Error vulkan_device_memory_find_memory(
//   unsigned *index, VkPhysicalDevice physical, unsigned filter,
//   VkMemoryPropertyFlags flags, VkMemoryPropertyFlags exclude
// ) {

// Error vulkan_device_memory_create(VkDeviceMemory *memory, VkDevice device, unsigned size, unsigned index) {

Error vulkan_buffer_create(VulkanBuffer *buffer, VkPhysicalDevice physical, VkDevice device, unsigned size, VkBufferUsageFlags usage, VkSharingMode sharing, unsigned *queues, unsigned length, VkMemoryPropertyFlags flags, VkMemoryPropertyFlags exclude) {
  if(!buffer) return NULL_HANDLE_ERROR;

  if(vulkan_raw_buffer_create(&buffer->handle, device, size, usage, sharing, queues, length) != SUCCESS)
    return VULKAN_RAW_BUFFER_CREATE_ERROR;

  VkMemoryRequirements requirement;
  vkGetBufferMemoryRequirements(device, buffer->handle, &requirement);

  unsigned index;
  if(vulkan_device_memory_find_memory(&index, physical, requirement.memoryTypeBits, flags, exclude) != SUCCESS)
    return VULKAN_DEVICE_MEMORY_FIND_MEMORY_ERROR;

  if(vulkan_device_memory_create(&buffer->memory, device, requirement.size, index) != SUCCESS)
    return VULKAN_DEVICE_MEMORY_ALLOCATE_ERROR;

  if(vkBindBufferMemory(device, buffer->handle, buffer->memory, 0) != VK_SUCCESS)
    return VULKAN_RAW_BUFFER_BIND_MEMORY_ERROR;

  return SUCCESS;
}

Error vulkan_buffers_stage(VulkanBuffer *buffers, VkPhysicalDevice physical, VkDevice device, VkQueue queue, VkCommandBuffer command, const void **datas, unsigned *sizes, unsigned length) {
  VulkanBuffer stages[length];

  if(vkResetCommandBuffer(command, 0) != VK_SUCCESS)
    return VULKAN_COMMAND_BUFFER_RESET_ERROR;

  if(vulkan_command_buffer_begin(command, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) != SUCCESS)
    return VULKAN_COMMAND_BUFFER_BEGIN_ERROR;

  VkBufferCopy regions[length];

  void *mapped;

  for(unsigned index = 0; index < length; ++index) {
    if(vulkan_buffer_create(stages + index, physical, device, sizes[index], VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE, NULL, 0, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, 0) != SUCCESS)
      return VULKAN_BUFFER_CREATE_ERROR;

    if(vkMapMemory(device, stages[index].memory, 0, sizes[index], 0, &mapped) != VK_SUCCESS)
      return VULKAN_DEVICE_MEMORY_MAP_ERROR;

    memcpy(mapped, datas[index], sizes[index]);
    vkUnmapMemory(device, stages[index].memory);

    regions[index] = (VkBufferCopy){
      .srcOffset = 0,
      .dstOffset = 0,
      .size = sizes[index],
    };

    vkCmdCopyBuffer(command, stages[index].handle, buffers[index].handle, 1, regions + index);
  }

  if(vkEndCommandBuffer(command) != VK_SUCCESS)
    return VULKAN_COMMAND_BUFFER_END_ERROR;

  if(vulkan_queue_submit(queue, command, NULL, NULL, 0, NULL, 0, NULL) != SUCCESS)
    return VULKAN_QUEUE_SUBMIT_ERROR;

  if(vkQueueWaitIdle(queue) != VK_SUCCESS)
    return VULKAN_QUEUE_WAIT_IDLE_ERROR;

  for(unsigned index = 0; index < length; ++index)
    vulkan_buffer_destroy(stages + index, device);

  return SUCCESS;
}

void vulkan_buffer_destroy(VulkanBuffer *buffer, VkDevice device) {
  if(!buffer) return;

  vkFreeMemory(device, buffer->memory, NULL);
  vkDestroyBuffer(device, buffer->handle, NULL);
}
