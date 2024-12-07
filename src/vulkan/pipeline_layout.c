#include "vulkan/pipeline_layout.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

Error vulkan_pipeline_layout_create(VkPipelineLayout *layout, VkDevice device) {
  VkPipelineLayoutCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .setLayoutCount = 0,
    .pSetLayouts = NULL,
    .pushConstantRangeCount = 0,
    .pPushConstantRanges = NULL,
  };

  if(vkCreatePipelineLayout(device, &info, NULL, layout) != VK_SUCCESS)
    return VULKAN_PIPELINE_LAYOUT_CREATE_ERROR;

  return SUCCESS;
}
