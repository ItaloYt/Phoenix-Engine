#ifndef PHOENIX_VULKAN_QUEUE_H
#define PHOENIX_VULKAN_QUEUE_H

#include "util/error.h"
#include "vulkan/swapchain.h"
#include <vulkan/vulkan_core.h>

Error vulkan_queue_submit(VkQueue queue, VkCommandBuffer buffer, const VkSemaphore *wait, const VkPipelineStageFlags *stages, unsigned wait_length, const VkSemaphore *signal, unsigned signal_length, VkFence fence);
Error vulkan_queue_present(VkQueue queue, const VulkanSwapchain *swapchain, const VkSemaphore *wait, unsigned wait_length);

#endif
