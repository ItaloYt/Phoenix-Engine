#ifndef PHOENIX_RENDERER_VULKAN_FRAMEBUFFERS_H
#define PHOENIX_RENDERER_VULKAN_FRAMEBUFFERS_H

#include "renderer/vulkan/device.h"
#include "renderer/vulkan/image_views.h"
#include "renderer/vulkan/render_pass.h"
#include "util/error.h"
#include <vulkan/vulkan_core.h>

typedef struct Framebuffers *Framebuffers;

Error framebuffers_create(Framebuffers *framebuffers, RenderPass render_pass, ImageViews views);
void framebuffers_destroy(Framebuffers framebuffers);

RenderPass framebuffers_get_render_pass(Framebuffers framebuffers);
ImageViews framebuffers_get_image_views(Framebuffers framebuffers);
Device framebuffers_get_device(Framebuffers framebuffers);
const VkFramebuffer *framebuffers_get_handles(Framebuffers framebuffers);
unsigned framebuffers_get_length(Framebuffers framebuffers);

#endif
