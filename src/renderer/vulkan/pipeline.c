#include "renderer/vulkan/pipeline.h"
#include "renderer/vulkan/device.h"
#include "renderer/vulkan/pipeline_layout.h"
#include "renderer/vulkan/render_pass.h"
#include "renderer/vulkan/shader_module.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

struct Pipeline {
  PipelineLayout layout;
  RenderPass render_pass;
  ShaderModule vertex_shader;
  ShaderModule fragment_shader;
  Device device;
  VkPipeline handle;
};

Error pipeline_create(Pipeline *pipeline, PipelineLayout layout, RenderPass render_pass, ShaderModule vertex_shader, ShaderModule fragment_shader) {
  if(!pipeline || !layout || !vertex_shader || !fragment_shader) return NULL_HANDLE_ERROR;

  const Device device = pipeline_layout_get_device(layout);
  const VkDevice vk_device = device_get_handle(device);
  const VkShaderModule vk_vertex_shader = shader_module_get_handle(vertex_shader);
  const VkShaderModule vk_fragment_shader = shader_module_get_handle(fragment_shader);
  const VkPipelineLayout vk_layout = pipeline_layout_get_handle(layout);
  const VkRenderPass vk_render_pass = render_pass_get_handle(render_pass);

  *pipeline = malloc(sizeof(struct Pipeline));
  if(!*pipeline) return ALLOCATION_ERROR;

  (*pipeline)->layout = layout;
  (*pipeline)->render_pass = render_pass;
  (*pipeline)->vertex_shader = vertex_shader;
  (*pipeline)->fragment_shader = fragment_shader;
  (*pipeline)->device = device;

  // TODO: Adjust this so we can resize the window
  // TODO: Create a depth testing state
  VkGraphicsPipelineCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .stageCount = 2,
    .pStages = (VkPipelineShaderStageCreateInfo[]){
      (VkPipelineShaderStageCreateInfo){
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .stage = VK_SHADER_STAGE_VERTEX_BIT,
        .module = vk_vertex_shader,
        .pName = "main",
        .pSpecializationInfo = NULL,
      },
      (VkPipelineShaderStageCreateInfo){
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
        .module = vk_fragment_shader,
        .pName = "main",
        .pSpecializationInfo = NULL,
      },
    },
    .pVertexInputState = &(VkPipelineVertexInputStateCreateInfo){
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .vertexBindingDescriptionCount = 0,
      .pVertexBindingDescriptions = NULL,
      .vertexAttributeDescriptionCount = 0,
      .pVertexAttributeDescriptions = NULL,
    },
    .pInputAssemblyState = &(VkPipelineInputAssemblyStateCreateInfo){
      .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
      .primitiveRestartEnable = VK_FALSE,
    },
    .pTessellationState = NULL,
    .pViewportState = &(VkPipelineViewportStateCreateInfo){
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .viewportCount = 1,
      .pViewports = NULL,
      .scissorCount = 1,
      .pScissors = NULL,
    },
    .pRasterizationState = &(VkPipelineRasterizationStateCreateInfo){
      .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .depthClampEnable = VK_FALSE,
      .rasterizerDiscardEnable = VK_FALSE,
      .polygonMode = VK_POLYGON_MODE_FILL,
      .cullMode = VK_CULL_MODE_BACK_BIT,
      .frontFace = VK_FRONT_FACE_CLOCKWISE,
      .depthBiasEnable = VK_FALSE,
      .depthBiasConstantFactor = 0,
      .depthBiasClamp = 0,
      .depthBiasSlopeFactor = 0,
      .lineWidth = 1,
    },
    .pMultisampleState = &(VkPipelineMultisampleStateCreateInfo){
      .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
      .sampleShadingEnable = VK_FALSE,
      .minSampleShading = 1,
      .pSampleMask = NULL,
      .alphaToCoverageEnable = VK_FALSE,
      .alphaToOneEnable = VK_FALSE,
    },
    .pDepthStencilState = NULL,
    .pColorBlendState = &(VkPipelineColorBlendStateCreateInfo){
      .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .logicOpEnable = VK_FALSE,
      .logicOp = VK_LOGIC_OP_COPY,
      .attachmentCount = 1,
      .pAttachments = (VkPipelineColorBlendAttachmentState[]){
        (VkPipelineColorBlendAttachmentState){
          .blendEnable = VK_FALSE,
          .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
          .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
          .colorBlendOp = VK_BLEND_OP_ADD,
          .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
          .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
          .alphaBlendOp = VK_BLEND_OP_ADD,
          .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
        },
      },
      .blendConstants = { 0, 0, 0, 0 },
    },
    .pDynamicState = &(VkPipelineDynamicStateCreateInfo){
      .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .dynamicStateCount = 2,
      .pDynamicStates = (VkDynamicState[]){
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_VIEWPORT,
      },
    },
    .layout = vk_layout,
    .renderPass = vk_render_pass,
    .subpass = 0,
    .basePipelineHandle = NULL,
    .basePipelineIndex = -1,
  };

  if(vkCreateGraphicsPipelines(vk_device, NULL, 1, &info, NULL, &(*pipeline)->handle) != VK_SUCCESS)
    return PIPELINE_CREATE_ERROR;

  return SUCCESS;
}

void pipeline_destroy(Pipeline pipeline) {
  if(!pipeline) return;

  const VkDevice vk_device = device_get_handle(pipeline->device);

  vkDestroyPipeline(vk_device, pipeline->handle, NULL);

  free(pipeline);
}

PipelineLayout pipeline_get_pipeline_layout(Pipeline pipeline) { return (!pipeline ? NULL : pipeline->layout); }
RenderPass pipeline_get_render_pass(Pipeline pipeline) { return (!pipeline ? NULL : pipeline->render_pass); }
ShaderModule pipeline_get_vertex_shader(Pipeline pipeline) { return (!pipeline ? NULL : pipeline->vertex_shader); }
ShaderModule pipeline_get_fragment_shader(Pipeline pipeline) { return (!pipeline ? NULL : pipeline->fragment_shader); }
Device pipeline_get_device(Pipeline pipeline) { return (!pipeline ? NULL : pipeline->device); }
VkPipeline pipeline_get_handle(Pipeline pipeline) { return (!pipeline ? NULL : pipeline->handle); }
