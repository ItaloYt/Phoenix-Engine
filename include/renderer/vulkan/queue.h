#ifndef PHOENIX_RENDERER_VULKAN_QUEUE_H
#define PHOENIX_RENDERER_VULKAN_QUEUE_H

#include "renderer/vulkan/command_buffer.h"
#include "renderer/vulkan/device.h"
#include "renderer/vulkan/fence.h"
#include "renderer/vulkan/semaphore.h"
#include "renderer/vulkan/swapchain.h"
#include "util/error.h"
#include <vulkan/vulkan_core.h>

typedef struct Queue *Queue;

Error queue_create(Queue *queue, Device device, unsigned family_index);
Error queue_submit(Queue queue, CommandBuffer buffer, const Semaphore *wait, const VkPipelineStageFlags *stages, unsigned wait_length, const Semaphore *signal, unsigned signal_length, Fence fence);
Error queue_present(Queue queue, Swapchain swapchain, Semaphore *wait, unsigned wait_length);
void queue_destroy(Queue queue);

Device queue_get_device(Queue queue);
VkQueue queue_get_handle(Queue queue);

#endif
