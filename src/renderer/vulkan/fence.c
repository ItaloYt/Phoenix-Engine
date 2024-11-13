#include "renderer/vulkan/fence.h"
#include "renderer/vulkan/device.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

struct Fence {
  Device device;
  VkFence handle;
};

Error fence_create(Fence *fence, Device device) {
  if(!fence || !device) return NULL_HANDLE_ERROR;

  const VkDevice vk_device = device_get_handle(device);

  *fence = malloc(sizeof(struct Fence));
  if(!*fence) return NULL_HANDLE_ERROR;

  (*fence)->device = device;

  VkFenceCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
    .pNext = NULL,
    .flags = VK_FENCE_CREATE_SIGNALED_BIT,
  };

  if(vkCreateFence(vk_device, &info, NULL, &(*fence)->handle) != VK_SUCCESS)
    return FENCE_CREATE_ERROR;

  return SUCCESS;
}

Error fences_wait(Fence *fences, unsigned length) {
  if(length == 0) return SUCCESS;

  const VkDevice vk_device = device_get_handle(fences[0]->device);

  VkFence vk_fences[length];
  for(unsigned index = 0; index < length; ++index)
    vk_fences[index] = fences[index]->handle;

  if(vkWaitForFences(vk_device, length, vk_fences, VK_TRUE, -1) != VK_SUCCESS)
    return FENCES_WAIT_ERROR;

  return SUCCESS;
}

Error fences_reset(Fence *fences, unsigned length) {
  if(length == 0) return SUCCESS;

  const VkDevice vk_device = device_get_handle(fences[0]->device);

  VkFence vk_fences[length];
  for(unsigned index = 0; index < length; ++index)
    vk_fences[index] = fences[index]->handle;

  if(vkResetFences(vk_device, length, vk_fences) != VK_SUCCESS)
    return FENCES_RESET_ERROR;

  return SUCCESS;
}

void fence_destroy(Fence fence) {
  if(!fence) return;

  const VkDevice vk_device = device_get_handle(fence->device);

  vkDestroyFence(vk_device, fence->handle, NULL);

  free(fence);
}

Device fence_get_device(Fence fence) { return (!fence ? NULL : fence->device); }
VkFence fence_get_handle(Fence fence) { return (!fence ? NULL : fence->handle); }
