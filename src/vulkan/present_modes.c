#include "vulkan/present_modes.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

Error vulkan_present_modes_create(VulkanPresentModes *modes, VkPhysicalDevice device, VkSurfaceKHR surface) {
  if(!modes) return NULL_HANDLE_ERROR;

  if(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &modes->length, NULL) != VK_SUCCESS)
    return VULKAN_DEVICE_GET_PRESENT_MODES_ERROR;

  void *tmp = realloc(modes->handles, modes->length * sizeof(VkPresentModeKHR));
  if(!tmp) return ALLOCATION_ERROR;
  modes->handles = tmp;

  if(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &modes->length, modes->handles) != VK_SUCCESS)
    return VULKAN_DEVICE_GET_PRESENT_MODES_ERROR;

  return SUCCESS;
}

void vulkan_present_modes_destroy(VulkanPresentModes *modes) {
  if(!modes) return;

  free(modes->handles);
  modes->handles = 0x0;
}
