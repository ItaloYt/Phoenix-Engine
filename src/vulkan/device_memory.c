#include "vulkan/device_memory.h"
#include "util/error.h"
#include <vulkan/vulkan_core.h>

Error vulkan_device_memory_create(VkDeviceMemory *memory, VkDevice device, unsigned size, unsigned index) {
  VkMemoryAllocateInfo info = {
    .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    .pNext = NULL,
    .allocationSize = size,
    .memoryTypeIndex = index,
  };

  if(vkAllocateMemory(device, &info, NULL, memory) != VK_SUCCESS)
    return VULKAN_DEVICE_MEMORY_ALLOCATE_ERROR;

  return SUCCESS;
}

Error vulkan_device_memory_find_memory(unsigned *index, VkPhysicalDevice physical, unsigned filter, VkMemoryPropertyFlags flags, VkMemoryPropertyFlags exclude) {
  if(!index) return NULL_HANDLE_ERROR;

  VkPhysicalDeviceMemoryProperties properties;
  vkGetPhysicalDeviceMemoryProperties(physical, &properties);

  for(*index = 0; *index < properties.memoryTypeCount; ++*index) {
    if((filter & (1 << *index)) && (properties.memoryTypes[*index].propertyFlags & flags) == flags && !(properties.memoryTypes[*index].propertyFlags & exclude))
        return SUCCESS;
  }

  return VULKAN_DEVICE_MEMORY_FIND_MEMORY_ERROR;
}
