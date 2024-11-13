#include "renderer/vulkan/render_pass.h"
#include "renderer/vulkan/command_buffer.h"
#include "renderer/vulkan/device.h"
#include "renderer/vulkan/swapchain.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

struct RenderPass {
  Swapchain swapchain;
  Device device;
  VkRenderPass handle;
};

Error render_pass_create(RenderPass *render_pass, Swapchain swapchain) {
  if(!render_pass || !swapchain) return NULL_HANDLE_ERROR;

  const Device device = swapchain_get_device(swapchain);
  const VkDevice vk_device = device_get_handle(device);
  const VkSurfaceFormatKHR *const format = swapchain_get_surface_format(swapchain);

  *render_pass = malloc(sizeof(struct RenderPass));
  if(!*render_pass) return ALLOCATION_ERROR;

  (*render_pass)->swapchain = swapchain;
  (*render_pass)->device = device;

  VkRenderPassCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .attachmentCount = 1,
    .pAttachments = (VkAttachmentDescription[]){
      (VkAttachmentDescription){
        .flags = 0,
        .format = format->format,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
      },
    },
    .subpassCount = 1,
    .pSubpasses = (VkSubpassDescription[]){
      (VkSubpassDescription){
        .flags = 0,
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .inputAttachmentCount = 0,
        .pInputAttachments = NULL,
        .colorAttachmentCount = 1,
        .pColorAttachments = (VkAttachmentReference[]){
          (VkAttachmentReference){
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
          },
        },
        .pResolveAttachments = NULL,
        .pDepthStencilAttachment = NULL,
        .preserveAttachmentCount = 0,
        .pPreserveAttachments = NULL,
      },
    },
    .dependencyCount = 1,
    .pDependencies = (VkSubpassDependency[]){
      (VkSubpassDependency){
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        .dependencyFlags = 0,
      },
    },
  };

  if(vkCreateRenderPass(vk_device, &info, NULL, &(*render_pass)->handle) != VK_SUCCESS)
    return RENDER_PASS_CREATE_ERROR;

  return SUCCESS;
}

void render_pass_begin(RenderPass render_pass, CommandBuffer buffer, VkFramebuffer framebuffer) {
  if(!render_pass || !buffer) return;

  const VkExtent2D *extent = swapchain_get_extent(render_pass->swapchain);

  const VkCommandBuffer vk_buffer = command_buffer_get_handle(buffer);

  VkRenderPassBeginInfo info = {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
    .pNext = NULL,
    .renderPass = render_pass->handle,
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

  vkCmdBeginRenderPass(vk_buffer, &info, VK_SUBPASS_CONTENTS_INLINE);
}

void render_pass_destroy(RenderPass render_pass) {
  if(!render_pass) return;

  const VkDevice vk_device = device_get_handle(render_pass->device);

  vkDestroyRenderPass(vk_device, render_pass->handle, NULL);

  free(render_pass);
}

Swapchain render_pass_get_swapchain(RenderPass render_pass) { return (!render_pass ? NULL : render_pass->swapchain); }
Device render_pass_get_device(RenderPass render_pass) { return (!render_pass ? NULL : render_pass->device); }
VkRenderPass render_pass_get_handle(RenderPass render_pass) { return (!render_pass ? NULL : render_pass->handle); }
