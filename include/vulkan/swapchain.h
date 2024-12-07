#ifndef PHOENIX_VULKAN_SWAPCHAIN_H
#define PHOENIX_VULKAN_SWAPCHAIN_H

#include "util/error.h"
#include <vulkan/vulkan_core.h>

typedef struct VulkanSwapchain {
  VkSwapchainKHR handle;
  unsigned image_index;
} VulkanSwapchain;

Error vulkan_swapchain_create(VulkanSwapchain *swapchain, VkDevice device, VkSurfaceKHR surface, const VkSurfaceFormatKHR *format, VkPresentModeKHR mode, const VkExtent2D *extent, unsigned image_count, VkSharingMode sharing, unsigned *indices, unsigned length);
Error vulkan_swapchain_acquire_image(VulkanSwapchain *swapchain, VkDevice device, VkSemaphore semaphore, VkFence fence);
void vulkan_swapchain_destroy(VulkanSwapchain *swapchain, VkDevice device);

#endif
