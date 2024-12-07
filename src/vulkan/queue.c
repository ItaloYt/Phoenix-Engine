#include "vulkan/queue.h"
#include "util/error.h"
#include "vulkan/swapchain.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

Error vulkan_queue_submit(VkQueue queue, VkCommandBuffer buffer, const VkSemaphore *wait, const VkPipelineStageFlags *stages, unsigned wait_length, const VkSemaphore *signal, unsigned signal_length, VkFence fence) {
  VkSubmitInfo info = {
    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .pNext = NULL,
    .waitSemaphoreCount = wait_length,
    .pWaitSemaphores = wait,
    .pWaitDstStageMask = stages,
    .commandBufferCount = 1,
    .pCommandBuffers = (VkCommandBuffer[]){ buffer },
    .signalSemaphoreCount = signal_length,
    .pSignalSemaphores = signal,
  };

  if(vkQueueSubmit(queue, 1, (VkSubmitInfo[]){ info }, fence) != VK_SUCCESS)
    return VULKAN_QUEUE_SUBMIT_ERROR;

  return SUCCESS;
}

Error vulkan_queue_present(VkQueue queue, const VulkanSwapchain *swapchain, const VkSemaphore *wait, unsigned wait_length) {
  VkPresentInfoKHR info = {
    .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
    .pNext = NULL,
    .waitSemaphoreCount = wait_length,
    .pWaitSemaphores = wait,
    .swapchainCount = 1,
    .pSwapchains = (VkSwapchainKHR[]){ swapchain->handle },
    .pImageIndices = (unsigned[]){ swapchain->image_index },
    .pResults = NULL,
  };

  switch(vkQueuePresentKHR(queue, &info)) {
    case SUCCESS:
      break;

    case VK_SUBOPTIMAL_KHR:
    case VK_ERROR_OUT_OF_DATE_KHR:
      return VULKAN_SWAPCHAIN_OUT_OF_DATE_ERROR;

    default:
      return VULKAN_QUEUE_PRESENT_ERROR;
  }

  return SUCCESS;
}
