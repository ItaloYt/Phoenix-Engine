#include "renderer/vulkan/queue.h"
#include "renderer/vulkan/command_buffer.h"
#include "renderer/vulkan/device.h"
#include "renderer/vulkan/fence.h"
#include "renderer/vulkan/semaphore.h"
#include "renderer/vulkan/swapchain.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

struct Queue {
  Device device;
  VkQueue handle;
};

Error queue_create(Queue *queue, Device device, unsigned family_index) {
  if(!queue || !device) return NULL_HANDLE_ERROR;

  const VkDevice vk_device = device_get_handle(device);

  *queue = malloc(sizeof(struct Queue));
  if(!*queue) return ALLOCATION_ERROR;

  (*queue)->device = device;
  vkGetDeviceQueue(vk_device, family_index, 0, &(*queue)->handle);

  return SUCCESS;
}

Error queue_submit(Queue queue, CommandBuffer buffer, const Semaphore *wait, const VkPipelineStageFlags *stages, unsigned wait_length, const Semaphore *signal, unsigned signal_length, Fence fence) {
  if(!queue || !buffer) return NULL_HANDLE_ERROR;

  const VkCommandBuffer vk_buffer = command_buffer_get_handle(buffer);
  const VkFence vk_fence = fence_get_handle(fence);

  VkSemaphore vk_wait[wait_length];
  for(unsigned index = 0; index < wait_length; ++index)
    vk_wait[index] = semaphore_get_handle(wait[index]);

  VkSemaphore vk_signal[signal_length];
  for(unsigned index = 0; index < signal_length; ++index)
    vk_signal[index] = semaphore_get_handle(signal[index]);

  VkSubmitInfo info = {
    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .pNext = NULL,
    .waitSemaphoreCount = wait_length,
    .pWaitSemaphores = vk_wait,
    .pWaitDstStageMask = stages,
    .commandBufferCount = 1,
    .pCommandBuffers = (VkCommandBuffer[]){ vk_buffer },
    .signalSemaphoreCount = signal_length,
    .pSignalSemaphores = vk_signal,
  };

  if(vkQueueSubmit(queue->handle, 1, (VkSubmitInfo[]){ info }, vk_fence) != VK_SUCCESS)
    return QUEUE_SUBMIT_ERROR;

  return SUCCESS;
}

Error queue_present(Queue queue, Swapchain swapchain, Semaphore *wait, unsigned wait_length) {
  if(!queue || !swapchain) return NULL_HANDLE_ERROR;

  const VkSwapchainKHR vk_swapchain = swapchain_get_handle(swapchain);
  const unsigned image_index = swapchain_get_image_index(swapchain);

  VkSemaphore vk_wait[wait_length];
  for(unsigned index = 0; index < wait_length; ++index)
    vk_wait[index] = semaphore_get_handle(wait[index]);

  VkPresentInfoKHR info = {
    .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
    .pNext = NULL,
    .waitSemaphoreCount = wait_length,
    .pWaitSemaphores = vk_wait,
    .swapchainCount = 1,
    .pSwapchains = (VkSwapchainKHR[]){ vk_swapchain },
    .pImageIndices = (unsigned[]){ image_index },
    .pResults = NULL,
  };

  if(vkQueuePresentKHR(queue->handle, &info) != VK_SUCCESS)
    return QUEUE_PRESENT_ERROR;

  return SUCCESS;
}

void queue_destroy(Queue queue) {
  if(!queue) return;

  free(queue);
}

Device queue_get_device(Queue queue) { return (!queue ? NULL : queue->device); }
VkQueue queue_get_handle(Queue queue) { return (!queue ? NULL : queue->handle); }
