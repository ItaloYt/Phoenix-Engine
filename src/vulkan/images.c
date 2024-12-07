#include "vulkan/images.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

Error vulkan_images_create(VulkanImages *images, VkDevice device, VkSwapchainKHR swapchain) {
  if(!images) return NULL_HANDLE_ERROR;

  if(vkGetSwapchainImagesKHR(device, swapchain, &images->length, NULL) != VK_SUCCESS)
    return VULKAN_IMAGES_GET_ERROR;

  void *tmp = realloc(images->handles, images->length * sizeof(VkImage));
  if(!tmp) return ALLOCATION_ERROR;
  images->handles = tmp;

  if(vkGetSwapchainImagesKHR(device, swapchain, &images->length, images->handles) != VK_SUCCESS)
    return VULKAN_IMAGES_GET_ERROR;

  return SUCCESS;
}

void vulkan_images_destroy(VulkanImages *images) {
  if(!images) return;

  free(images->handles);
  images->handles = 0x0;
}
