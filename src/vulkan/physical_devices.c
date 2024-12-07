#include "vulkan/physical_devices.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

Error vulkan_physical_devices_create(VulkanPhysicalDevices *devices, VkInstance instance) {
  if(!devices) return NULL_HANDLE_ERROR;

  if(vkEnumeratePhysicalDevices(instance, &devices->length, NULL) != VK_SUCCESS)
    return VULKAN_PHYSICALS_ENUMERATE_ERROR;

  void *tmp = realloc(devices->handles, devices->length * sizeof(VkPhysicalDevice));
  if(!tmp) return ALLOCATION_ERROR;
  devices->handles = tmp;

  if(vkEnumeratePhysicalDevices(instance, &devices->length, devices->handles) != VK_SUCCESS)
    return VULKAN_PHYSICALS_ENUMERATE_ERROR;

  return SUCCESS;
}

void vulkan_physical_devices_destroy(VulkanPhysicalDevices *devices) {
  if(!devices) return;

  free(devices->handles);
  devices->handles = 0x0;
}
