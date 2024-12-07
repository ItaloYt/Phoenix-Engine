#include "vulkan/command_pool.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

Error vulkan_command_pool_create(VkCommandPool *pool, VkDevice device, unsigned index) {
  VkCommandPoolCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .pNext = NULL,
    .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
    .queueFamilyIndex = index,
  };

  if(vkCreateCommandPool(device, &info, NULL, pool) != VK_SUCCESS)
    return VULKAN_COMMAND_POOL_CREATE_ERROR;

  return SUCCESS;
}
