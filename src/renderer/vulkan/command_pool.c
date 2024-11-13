#include "renderer/vulkan/command_pool.h"
#include "renderer/vulkan/device.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

struct CommandPool {
  Device device;
  VkCommandPool handle;
};

Error command_pool_create(CommandPool *pool, Device device, unsigned family_index) {
  if(!pool || !device) return NULL_HANDLE_ERROR;

  const VkDevice vk_device = device_get_handle(device);

  *pool = malloc(sizeof(struct CommandPool));
  if(!*pool) return ALLOCATION_ERROR;

  (*pool)->device = device;

  VkCommandPoolCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .pNext = NULL,
    .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
    .queueFamilyIndex = family_index,
  };

  if(vkCreateCommandPool(vk_device, &info, NULL, &(*pool)->handle) != VK_SUCCESS)
    return COMMAND_POOL_CREATE_ERROR;

  return SUCCESS;
}

void command_pool_destroy(CommandPool pool) {
  if(!pool) return;

  const VkDevice vk_device = device_get_handle(pool->device);

  vkDestroyCommandPool(vk_device, pool->handle, NULL);

  free(pool);
}

Device command_pool_get_device(CommandPool pool) { return (!pool ? NULL : pool->device); }
VkCommandPool command_pool_get_handle(CommandPool pool) { return (!pool ? NULL : pool->handle); }
