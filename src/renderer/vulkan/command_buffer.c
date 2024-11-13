#include "renderer/vulkan/command_buffer.h"
#include "renderer/vulkan/command_pool.h"
#include "renderer/vulkan/device.h"
#include "renderer/vulkan/framebuffers.h"
#include "renderer/vulkan/render_pass.h"
#include "renderer/vulkan/swapchain.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

struct CommandBuffer {
  CommandPool pool;
  Device device;
  VkCommandBuffer handle;
};

Error command_buffer_create(CommandBuffer *buffer, CommandPool pool) {
  if(!buffer || !pool) return NULL_HANDLE_ERROR;

  const VkCommandPool vk_pool = command_pool_get_handle(pool);
  const Device device = command_pool_get_device(pool);
  const VkDevice vk_device = device_get_handle(device);

  *buffer = malloc(sizeof(struct CommandBuffer));
  if(!*buffer) return ALLOCATION_ERROR;

  (*buffer)->pool = pool;
  (*buffer)->device = device;

  VkCommandBufferAllocateInfo info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    .pNext = NULL,
    .commandPool = vk_pool,
    .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    .commandBufferCount = 1,
  };

  if(vkAllocateCommandBuffers(vk_device, &info, &(*buffer)->handle) != VK_SUCCESS)
    return COMMAND_BUFFER_CREATE_ERROR;

  return SUCCESS;
}

Error command_buffer_begin(CommandBuffer buffer, RenderPass render_pass, Framebuffers framebuffers, unsigned index) {
  if(!buffer) return NULL_HANDLE_ERROR;

  if(vkResetCommandBuffer(buffer->handle, 0) != VK_SUCCESS)
    return COMMAND_BUFFER_RESET_ERROR;

  VkCommandBufferBeginInfo info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .pNext = NULL,
    .flags = 0,
    .pInheritanceInfo = NULL,
  };

  if(vkBeginCommandBuffer(buffer->handle, &info) != VK_SUCCESS)
    return COMMAND_BUFFER_BEGIN_ERROR;

  const VkRenderPass vk_render_pass = render_pass_get_handle(render_pass);
  const Swapchain swapchain = render_pass_get_swapchain(render_pass);
  const VkExtent2D *extent = swapchain_get_extent(swapchain);
  const VkFramebuffer *vk_framebuffers = framebuffers_get_handles(framebuffers);

  VkRenderPassBeginInfo rinfo = {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
    .pNext = NULL,
    .renderPass = vk_render_pass,
    .framebuffer = vk_framebuffers[index],
    .renderArea = {
      .offset = { 0, 0 },
      .extent = *extent,
    },
    .clearValueCount = 1,
    .pClearValues = (VkClearValue[]){
      (VkClearValue){
        .color = { { 0, 0, 0, 1 } },
      },
    },
  };

  vkCmdBeginRenderPass(buffer->handle, &rinfo, VK_SUBPASS_CONTENTS_INLINE);

  return SUCCESS;
}

Error command_buffer_end(CommandBuffer buffer) {
  if(!buffer) return NULL_HANDLE_ERROR;

  vkCmdEndRenderPass(buffer->handle);

  if(vkEndCommandBuffer(buffer->handle) != VK_SUCCESS)
    return COMMAND_BUFFER_END_ERROR;

  return SUCCESS;
}

// TODO: Is it better to create inside the CommandPool?
void command_buffer_destroy(CommandBuffer buffer) {
  if(!buffer) return;

  free(buffer);
}

CommandPool command_buffer_get_command_pool(CommandBuffer buffer) { return (!buffer ? NULL : buffer->pool); }
Device command_buffer_get_device(CommandBuffer buffer) { return (!buffer ? NULL : buffer->device); }
VkCommandBuffer command_buffer_get_handle(CommandBuffer buffer) { return (!buffer ? NULL : buffer->handle); }
