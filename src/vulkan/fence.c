#include "vulkan/fence.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

Error vulkan_fence_create(VkFence *fence, VkDevice device) {
  VkFenceCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
    .pNext = NULL,
    .flags = VK_FENCE_CREATE_SIGNALED_BIT,
  };

  if(vkCreateFence(device, &info, NULL, fence) != VK_SUCCESS)
    return VULKAN_FENCE_CREATE_ERROR;

  return SUCCESS;
}
