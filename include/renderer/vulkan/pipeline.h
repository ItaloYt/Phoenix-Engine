#ifndef PHOENIX_RENDERER_VULKAN_PIPELINE_H
#define PHOENIX_RENDERER_VULKAN_PIPELINE_H

#include "renderer/vulkan/device.h"
#include "renderer/vulkan/pipeline_layout.h"
#include "renderer/vulkan/render_pass.h"
#include "renderer/vulkan/shader_module.h"
#include "util/error.h"
#include <vulkan/vulkan_core.h>

typedef struct Pipeline *Pipeline;

Error pipeline_create(Pipeline *pipeline, PipelineLayout layout, RenderPass render_pass, ShaderModule vertex_shader, ShaderModule fragment_shader);
void pipeline_destroy(Pipeline pipeline);

PipelineLayout pipeline_get_pipeline_layout(Pipeline pipeline);
RenderPass pipeline_get_render_pass(Pipeline pipeline);
ShaderModule pipeline_get_vertex_shader(Pipeline pipeline);
ShaderModule pipeline_get_fragment_shader(Pipeline pipeline);
Device pipeline_get_device(Pipeline pipeline);
VkPipeline pipeline_get_handle(Pipeline pipeline);

#endif
