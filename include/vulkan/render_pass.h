#ifndef PHOENIX_VULKAN_RENDER_PASS_H
#define PHOENIX_VULKAN_RENDER_PASS_H

#include "util/error.h"
#include <vulkan/vulkan_core.h>

Error vulkan_render_pass_create(VkRenderPass *render_pass, VkDevice device, VkFormat format);

#endif
