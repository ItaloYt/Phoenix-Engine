#ifndef PHOENIX_RENDERER_VULKAN_RENDER_PASS_H
#define PHOENIX_RENDERER_VULKAN_RENDER_PASS_H

#include "renderer/vulkan/command_buffer.h"
#include "renderer/vulkan/device.h"
#include "renderer/vulkan/swapchain.h"
#include "util/error.h"
#include <vulkan/vulkan_core.h>

typedef struct RenderPass *RenderPass;

Error render_pass_create(RenderPass *render_pass, Swapchain swapchain);
void render_pass_begin(RenderPass render_pass, CommandBuffer buffer, VkFramebuffer framebuffer);
void render_pass_destroy(RenderPass render_pass);

Swapchain render_pass_get_swapchain(RenderPass render_pass);
Device render_pass_get_device(RenderPass render_pass);
VkRenderPass render_pass_get_handle(RenderPass render_pass);

#endif
