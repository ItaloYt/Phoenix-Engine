#ifndef PHOENIX_RENDERER_VULKAN_PIPELINE_LAYOUT_H
#define PHOENIX_RENDERER_VULKAN_PIPELINE_LAYOUT_H

#include "util/error.h"
#include <vulkan/vulkan_core.h>

Error vulkan_pipeline_layout_create(VkPipelineLayout *layout, VkDevice device);

#endif
