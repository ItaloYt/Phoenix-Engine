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
  Semaphore image_signal;
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

  if(semaphore_create(&(*swapchain)->image_signal, device) != SUCCESS)
    return SEMAPHORE_CREATE_ERROR;

  return SUCCESS;
}

Error swapchain_acquire_image(Swapchain swapchain, Fence fence) {
  if(!swapchain) return NULL_HANDLE_ERROR;

  const VkDevice vk_device = device_get_handle(swapchain->device);
  const VkSemaphore vk_image_signal = semaphore_get_handle(swapchain->image_signal);
  const VkFence vk_fence = fence_get_handle(fence);

  // printf("INFO: %p %p %p %p %p\n", vk_device, swapchain->handle, vk_image_signal, vk_fence, &swapchain->image_index);

  VkResult result = vkAcquireNextImageKHR(vk_device, swapchain->handle, -1, vk_image_signal, vk_fence, &swapchain->image_index);
  if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    printf("INFO: Result=%d\n", result);
    return SWAPCHAIN_ACQUIRE_IMAGE_ERROR;
  }

  return SUCCESS;
}

void swapchain_destroy(Swapchain swapchain) {
  if(!swapchain) return;

  const VkDevice vk_device = device_get_handle(swapchain->device);

  semaphore_destroy(swapchain->image_signal);

  vkDestroySwapchainKHR(vk_device, swapchain->handle, NULL);

  free(swapchain);
}

Device swapchain_get_device(Swapchain swapchain) { return (!swapchain ? NULL : swapchain->device); };
const VkSurfaceFormatKHR *swapchain_get_surface_format(Swapchain swapchain) { return (!swapchain ? NULL : swapchain->format); };
VkPresentModeKHR swapchain_get_present_mode(Swapchain swapchain) { return (!swapchain ? VK_PRESENT_MODE_FIFO_KHR : swapchain->mode); };
VkSwapchainKHR swapchain_get_handle(Swapchain swapchain) { return (!swapchain ? NULL : swapchain->handle); };
const VkExtent2D *swapchain_get_extent(Swapchain swapchain) { return (!swapchain ? NULL : &swapchain->extent); };
Semaphore swapchain_get_image_signal(Swapchain swapchain) { return (!swapchain ? NULL : swapchain->image_signal); }
unsigned swapchain_get_image_index(Swapchain swapchain) { return (!swapchain ? -1 : swapchain->image_index); }
