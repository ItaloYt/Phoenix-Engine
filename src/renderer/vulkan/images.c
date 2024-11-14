#include "renderer/vulkan/images.h"
#include "renderer/vulkan/device.h"
#include "renderer/vulkan/swapchain.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

struct Images {
  Swapchain swapchain;
  Device device;
  VkImage *handles;
  unsigned length;
};

Error images_create(Images *images, Swapchain swapchain) {
  if(!images || !swapchain) return NULL_HANDLE_ERROR;

  const Device device = swapchain_get_device(swapchain);
  const VkDevice vk_device = device_get_handle(device);
  const VkSwapchainKHR vk_swapchain = swapchain_get_handle(swapchain);

  *images = malloc(sizeof(struct Images));
  if(!*images) return ALLOCATION_ERROR;

  (*images)->swapchain = swapchain;
  (*images)->device = device;

  if(vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &(*images)->length, NULL) != VK_SUCCESS)
    return IMAGES_GET_ERROR;

  (*images)->handles = malloc((*images)->length * sizeof(VkImage));
  if(!(*images)->handles) return ALLOCATION_ERROR;

  if(vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &(*images)->length, (*images)->handles) != VK_SUCCESS)
    return IMAGES_GET_ERROR;

  return SUCCESS;
}

Error images_recreate(Images images) {
  if(!images) return NULL_HANDLE_ERROR;

  const VkDevice vk_device = device_get_handle(images->device);
  const VkSwapchainKHR vk_swapchain = swapchain_get_handle(images->swapchain);

  if(vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &images->length, NULL) != VK_SUCCESS)
    return IMAGES_GET_ERROR;

  void *tmp = realloc(images->handles, images->length * sizeof(VkImage));
  if(!tmp) return ALLOCATION_ERROR;

  images->handles = tmp;

  if(vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &images->length, images->handles) != VK_SUCCESS)
    return IMAGES_GET_ERROR;

  return SUCCESS;
}

void images_destroy(Images images) {
  if(!images) return;

  free(images->handles);
  free(images);
}

Swapchain images_get_swapchain(Images images) { return (!images ? NULL : images->swapchain); }
Device images_get_device(Images images) { return (!images ? NULL : images->device); }
const VkImage *images_get_handles(Images images) { return (!images ? NULL : images->handles); }
unsigned images_get_length(Images images) { return (!images ? 0 : images->length); }
