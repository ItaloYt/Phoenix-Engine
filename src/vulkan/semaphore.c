#include "vulkan/semaphore.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

Error vulkan_semaphore_create(VkSemaphore *semaphore, VkDevice device) {
  VkSemaphoreCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
  };

  if(vkCreateSemaphore(device, &info, NULL, semaphore) != VK_SUCCESS)
    return VULKAN_SEMAPHORE_CREATE_ERROR;

  return SUCCESS;
}
