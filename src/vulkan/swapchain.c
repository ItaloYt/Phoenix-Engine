#include "vulkan/swapchain.h"
#include "util/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

Error vulkan_swapchain_create(VulkanSwapchain *swapchain, VkDevice device, VkSurfaceKHR surface, const VkSurfaceFormatKHR *format, VkPresentModeKHR mode, const VkExtent2D *extent, unsigned image_count, VkSharingMode sharing, unsigned *indices, unsigned length) {
  if(!swapchain) return NULL_HANDLE_ERROR;

  const VkSwapchainKHR old_handle = swapchain->handle;

  VkSwapchainCreateInfoKHR info = {
    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .pNext = NULL,
    .flags = 0,
    .surface = surface,
    .minImageCount = image_count + 1,
    .imageFormat = format->format,
    .imageColorSpace = format->colorSpace,
    .imageExtent = *extent,
    .imageArrayLayers = 1,
    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    .imageSharingMode = sharing,
    .queueFamilyIndexCount = length,
    .pQueueFamilyIndices = indices,
    .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
    .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    .presentMode = mode,
    .clipped = VK_TRUE,
    .oldSwapchain = old_handle,
  };

  if(vkCreateSwapchainKHR(device, &info, NULL, &swapchain->handle) != VK_SUCCESS)
    return VULKAN_SWAPCHAIN_CREATE_ERROR;

  vkDestroySwapchainKHR(device, old_handle, NULL);

  return SUCCESS;
}

Error vulkan_swapchain_acquire_image(VulkanSwapchain *swapchain, VkDevice device, VkSemaphore semaphore, VkFence fence) {
  if(!swapchain) return NULL_HANDLE_ERROR;

  switch(vkAcquireNextImageKHR(device, swapchain->handle, -1, semaphore, fence, &swapchain->image_index)) {
    case SUCCESS:
      break;

    case VK_SUBOPTIMAL_KHR:
    case VK_ERROR_OUT_OF_DATE_KHR:
      return VULKAN_SWAPCHAIN_OUT_OF_DATE_ERROR;

    default:
      return VULKAN_SWAPCHAIN_ACQUIRE_IMAGE_ERROR;
  }

  return SUCCESS;
}

void vulkan_swapchain_destroy(VulkanSwapchain *swapchain, VkDevice device) {
  if(!swapchain) return;

  vkDestroySwapchainKHR(device, swapchain->handle, NULL);
  swapchain->handle = 0x0;
}
