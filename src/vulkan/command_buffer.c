#include "vulkan/command_buffer.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

Error vulkan_command_buffers_create(VkCommandBuffer *buffers, VkDevice device, VkCommandPool pool, unsigned length) {
  VkCommandBufferAllocateInfo info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    .pNext = NULL,
    .commandPool = pool,
    .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    .commandBufferCount = length,
  };

  if(vkAllocateCommandBuffers(device, &info, buffers) != VK_SUCCESS)
    return VULKAN_COMMAND_BUFFER_ALLOCATE_ERROR;

  return SUCCESS;
}

Error vulkan_command_buffer_begin(VkCommandBuffer buffer, VkCommandBufferUsageFlags flags) {
  VkCommandBufferBeginInfo info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .pNext = NULL,
    .flags = flags,
    .pInheritanceInfo = NULL,
  };

  if(vkBeginCommandBuffer(buffer, &info) != VK_SUCCESS)
    return VULKAN_COMMAND_BUFFER_BEGIN_ERROR;

  return SUCCESS;
}

void vulkan_command_buffer_begin_render_pass(VkCommandBuffer buffer, VkRenderPass render_pass, VkFramebuffer framebuffer, const VkExtent2D *extent) {
  VkRenderPassBeginInfo info = {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
    .pNext = NULL,
    .renderPass = render_pass,
    .framebuffer = framebuffer,
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

  vkCmdBeginRenderPass(buffer, &info, VK_SUBPASS_CONTENTS_INLINE);
}
