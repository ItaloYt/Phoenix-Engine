#include "renderer/vulkan/semaphore.h"
#include "renderer/vulkan/device.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

struct Semaphore {
  Device device;
  VkSemaphore handle;
};

Error semaphore_create(Semaphore *semaphore, Device device) {
  if(!semaphore || !device) return NULL_HANDLE_ERROR;

  const VkDevice vk_device = device_get_handle(device);

  *semaphore = malloc(sizeof(struct Semaphore));
  if(!*semaphore) return ALLOCATION_ERROR;

  (*semaphore)->device = device;

  VkSemaphoreCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
  };

  if(vkCreateSemaphore(vk_device, &info, NULL, &(*semaphore)->handle) != VK_SUCCESS)
    return SEMAPHORE_CREATE_ERROR;

  return SUCCESS;
}

void semaphore_destroy(Semaphore semaphore) {
  if(!semaphore) return;

  const VkDevice vk_device = device_get_handle(semaphore->device);

  vkDestroySemaphore(vk_device, semaphore->handle, NULL);

  free(semaphore);
}

Device semaphore_get_device(Semaphore semaphore) { return (!semaphore ? NULL : semaphore->device); }
VkSemaphore semaphore_get_handle(Semaphore semaphore) { return (!semaphore ? NULL : semaphore->handle); }
