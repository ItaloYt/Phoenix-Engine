#ifndef PHOENIX_RENDERER_GROUP_H
#define PHOENIX_RENDERER_GROUP_H

#include "renderer/vulkan/framebuffers.h"
#include "renderer/vulkan/pipeline.h"
#include "renderer/vulkan/pipeline_layout.h"
#include "renderer/vulkan/render_pass.h"
#include "renderer/vulkan/shader_module.h"

typedef struct {
  ShaderModule vertex_shader;
  ShaderModule fragment_shader;
  PipelineLayout layout;
  RenderPass render_pass;
  Framebuffers framebuffers;
  Pipeline pipeline;
} RenderingGroup;

#endif
