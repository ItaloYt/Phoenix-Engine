#include "renderer/vulkan/swapchain.h"
#include "renderer/vulkan/device.h"
#include "renderer/vulkan/fence.h"
#include "renderer/vulkan/instance.h"
#include "renderer/vulkan/semaphore.h"
#include "renderer/vulkan/surface.h"
#include "util/error.h"
#include "window.h"
#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

struct Swapchain {
  Device device;
  const VkSurfaceFormatKHR *format;
  VkPresentModeKHR mode;
  VkSwapchainKHR handle;
  VkExtent2D extent;
  unsigned image_index;
};

Error swapchain_create(Swapchain *swapchain, Device device, const VkSurfaceFormatKHR *format, VkPresentModeKHR mode) {
  if(!swapchain || !device || !format) return NULL_HANDLE_ERROR;

  const Surface surface = device_get_surface(device);
  const VkSurfaceCapabilitiesKHR *capabilities = device_get_capabilities(device);
  const VkDevice vk_device = device_get_handle(device);
  const VkSurfaceKHR vk_surface = surface_get_handle(surface);
  const unsigned graphic_family = device_get_graphic_family_index(device);
  const unsigned present_family = device_get_present_family_index(device);

  *swapchain = malloc(sizeof(struct Swapchain));
  if(!*swapchain) return ALLOCATION_ERROR;

  (*swapchain)->device = device;
  (*swapchain)->format = format;
  (*swapchain)->mode = mode;
  (*swapchain)->extent = capabilities->currentExtent;

  if((*swapchain)->extent.width == -1) {
    const Instance instance = surface_get_instance(surface);
    const Window window = instance_get_window(instance);
    window_get_framebuffer_size(window, &(*swapchain)->extent.width, &(*swapchain)->extent.height);
  }

  VkSwapchainCreateInfoKHR info = {
    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .pNext = NULL,
    .flags = 0,
    .surface = vk_surface,
    .minImageCount = capabilities->minImageCount + 1,
    .imageFormat = format->format,
    .imageColorSpace = format->colorSpace,
    .imageExtent = (*swapchain)->extent,
    .imageArrayLayers = 1,
    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    .imageSharingMode = VK_SHARING_MODE_CONCURRENT,
    .queueFamilyIndexCount = 2,
    .pQueueFamilyIndices = (unsigned[]){ graphic_family, present_family },
    .preTransform = capabilities->currentTransform,
    .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    .presentMode = mode,
    .clipped = VK_TRUE,
    .oldSwapchain = NULL,
  };

  if(graphic_family == present_family) {
    info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount = 0;
  }

  if(vkCreateSwapchainKHR(vk_device, &info, NULL, &(*swapchain)->handle) != VK_SUCCESS)
    return SWAPCHAIN_CREATE_ERROR;

  return SUCCESS;
}

Error swapchain_recreate(Swapchain swapchain, const VkSurfaceFormatKHR *format, VkPresentModeKHR mode) {
  if(!swapchain) return NULL_HANDLE_ERROR;

  const VkDevice vk_device = device_get_handle(swapchain->device);
  const Surface surface = device_get_surface(swapchain->device);
  const VkSurfaceKHR vk_surface = surface_get_handle(surface);
  const VkSurfaceCapabilitiesKHR *capabilities = device_get_capabilities(swapchain->device);
  const unsigned graphic_family = device_get_graphic_family_index(swapchain->device);
  const unsigned present_family = device_get_present_family_index(swapchain->device);
  const VkSwapchainKHR vk_old_swapchain = swapchain->handle;

  swapchain->extent = capabilities->currentExtent;
  swapchain->format = format;
  swapchain->mode = mode;

  if(swapchain->extent.width == -1) {
    const Instance instance = surface_get_instance(surface);
    const Window window = instance_get_window(instance);
    window_get_framebuffer_size(window, &swapchain->extent.width, &swapchain->extent.height);
  }

  VkSwapchainCreateInfoKHR info = {
    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .pNext = NULL,
    .flags = 0,
    .surface = vk_surface,
    .minImageCount = capabilities->minImageCount + 1,
    .imageFormat = format->format,
    .imageColorSpace = format->colorSpace,
    .imageExtent = swapchain->extent,
    .imageArrayLayers = 1,
    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .queueFamilyIndexCount = 2,
    .pQueueFamilyIndices = (unsigned[]){ graphic_family, present_family },
    .preTransform = capabilities->currentTransform,
    .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    .presentMode = mode,
    .clipped = VK_TRUE,
    .oldSwapchain = vk_old_swapchain,
  };

  if(graphic_family == present_family) {
    info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount = 0;
  }

  if(vkCreateSwapchainKHR(vk_device, &info, NULL, &swapchain->handle) != VK_SUCCESS)
    return SWAPCHAIN_CREATE_ERROR;

  vkDestroySwapchainKHR(vk_device, vk_old_swapchain, NULL);

  return SUCCESS;
}

Error swapchain_acquire_image(Swapchain swapchain, Semaphore semaphore, Fence fence) {
  if(!swapchain) return NULL_HANDLE_ERROR;

  const VkDevice vk_device = device_get_handle(swapchain->device);
  const VkSemaphore vk_semaphore = semaphore_get_handle(semaphore);
  const VkFence vk_fence = fence_get_handle(fence);

  switch(vkAcquireNextImageKHR(vk_device, swapchain->handle, -1, vk_semaphore, vk_fence, &swapchain->image_index)) {
    case SUCCESS:
      break;

    case VK_SUBOPTIMAL_KHR:
    case VK_ERROR_OUT_OF_DATE_KHR:
      return SWAPCHAIN_OUT_OF_DATE_ERROR;

    default:
      return SWAPCHAIN_ACQUIRE_IMAGE_ERROR;
  }

  return SUCCESS;
}

void swapchain_destroy(Swapchain swapchain) {
  if(!swapchain) return;

  const VkDevice vk_device = device_get_handle(swapchain->device);

  vkDestroySwapchainKHR(vk_device, swapchain->handle, NULL);

  free(swapchain);
}

Device swapchain_get_device(Swapchain swapchain) { return (!swapchain ? NULL : swapchain->device); };
const VkSurfaceFormatKHR *swapchain_get_surface_format(Swapchain swapchain) { return (!swapchain ? NULL : swapchain->format); };
VkPresentModeKHR swapchain_get_present_mode(Swapchain swapchain) { return (!swapchain ? VK_PRESENT_MODE_FIFO_KHR : swapchain->mode); };
VkSwapchainKHR swapchain_get_handle(Swapchain swapchain) { return (!swapchain ? NULL : swapchain->handle); };
const VkExtent2D *swapchain_get_extent(Swapchain swapchain) { return (!swapchain ? NULL : &swapchain->extent); };
unsigned swapchain_get_image_index(Swapchain swapchain) { return (!swapchain ? -1 : swapchain->image_index); }
