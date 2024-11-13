#ifndef PHOENIX_RENDERER_VULKAN_PIPELINE_LAYOUT_H
#define PHOENIX_RENDERER_VULKAN_PIPELINE_LAYOUT_H

#include "renderer/vulkan/device.h"
#include "util/error.h"
#include <vulkan/vulkan_core.h>

typedef struct PipelineLayout *PipelineLayout;

Error pipeline_layout_create(PipelineLayout *layout, Device device);
void pipeline_layout_destroy(PipelineLayout layout);

Device pipeline_layout_get_device(PipelineLayout layout);
VkPipelineLayout pipeline_layout_get_handle(PipelineLayout layout);

#endif
