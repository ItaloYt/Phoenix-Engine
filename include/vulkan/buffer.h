#ifndef PHOENIX_VULKAN_BUFFER_H
#define PHOENIX_VULKAN_BUFFER_H

#include "util/error.h"
#include <vulkan/vulkan_core.h>

typedef struct VulkanBuffer {
  VkBuffer handle;
  VkDeviceMemory memory;
} VulkanBuffer;

Error vulkan_buffer_create(VulkanBuffer *buffer, VkPhysicalDevice physical, VkDevice device, unsigned size, VkBufferUsageFlags usage, VkSharingMode sharing, unsigned *queues, unsigned length, VkMemoryPropertyFlags flags, VkMemoryPropertyFlags exclude);
Error vulkan_buffers_stage(VulkanBuffer *buffers, VkPhysicalDevice physical, VkDevice device, VkQueue queue, VkCommandBuffer command, const void **datas, unsigned *sizes, unsigned length);
void vulkan_buffer_destroy(VulkanBuffer *buffer, VkDevice device);

#endif
