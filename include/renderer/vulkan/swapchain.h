#ifndef PHOENIX_RENDERER_VULKAN_SWAPCHAIN_H
#define PHOENIX_RENDERER_VULKAN_SWAPCHAIN_H

#include "renderer/vulkan/device.h"
#include "renderer/vulkan/fence.h"
#include "renderer/vulkan/semaphore.h"
#include "util/error.h"
#include <vulkan/vulkan_core.h>

typedef struct Swapchain *Swapchain;

Error swapchain_create(Swapchain *swapchain, Device device, const VkSurfaceFormatKHR *format, VkPresentModeKHR mode);
Error swapchain_recreate(Swapchain swapchain, const VkSurfaceFormatKHR *format, VkPresentModeKHR mode);
Error swapchain_acquire_image(Swapchain swapchain, Semaphore semaphore, Fence fence);
void swapchain_destroy(Swapchain swapchain);
Device swapchain_get_device(Swapchain swapchain);
const VkSurfaceFormatKHR *swapchain_get_surface_format(Swapchain swapchain);
VkPresentModeKHR swapchain_get_present_mode(Swapchain swapchain);
VkSwapchainKHR swapchain_get_handle(Swapchain swapchain);
const VkExtent2D *swapchain_get_extent(Swapchain swapchain);
unsigned swapchain_get_image_index(Swapchain swapchain);

#endif
