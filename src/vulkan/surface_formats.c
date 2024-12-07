#include "vulkan/surface_formats.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

Error vulkan_surface_formats_create(VulkanSurfaceFormats *formats, VkPhysicalDevice device, VkSurfaceKHR surface) {
  if(!formats) return NULL_HANDLE_ERROR;

  if(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formats->length, NULL) != VK_SUCCESS)
    return VULKAN_DEVICE_GET_FORMATS_ERROR;

  void *tmp = realloc(formats->handles, formats->length * sizeof(VkSurfaceFormatKHR));
  if(!tmp) return ALLOCATION_ERROR;
  formats->handles = tmp;

  if(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formats->length, formats->handles) != VK_SUCCESS)
    return VULKAN_DEVICE_GET_FORMATS_ERROR;

  return SUCCESS;
}

void vulkan_surface_formats_destroy(VulkanSurfaceFormats *formats) {
  if(!formats) return;

  free(formats->handles);
  formats->handles = 0x0;
}
