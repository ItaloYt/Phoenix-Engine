#include "renderer/vulkan/pipeline_layout.h"
#include "renderer/vulkan/device.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

struct PipelineLayout {
  Device device;
  VkPipelineLayout handle;
};

Error pipeline_layout_create(PipelineLayout *layout, Device device) {
  if(!layout) return NULL_HANDLE_ERROR;

  const VkDevice vk_device = device_get_handle(device);

  *layout = malloc(sizeof(struct PipelineLayout));
  if(!*layout) return ALLOCATION_ERROR;

  (*layout)->device = device;

  VkPipelineLayoutCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .setLayoutCount = 0,
    .pSetLayouts = NULL,
    .pushConstantRangeCount = 0,
    .pPushConstantRanges = NULL,
  };

  if(vkCreatePipelineLayout(vk_device, &info, NULL, &(*layout)->handle) != VK_SUCCESS)
    return PIPELINE_LAYOUT_CREATE_ERROR;

  return SUCCESS;
}

void pipeline_layout_destroy(PipelineLayout layout) {
  if(!layout) return;

  const VkDevice vk_device = device_get_handle(layout->device);

  vkDestroyPipelineLayout(vk_device, layout->handle, NULL);

  free(layout);
}

Device pipeline_layout_get_device(PipelineLayout layout) { return (!layout ? NULL : layout->device); }
VkPipelineLayout pipeline_layout_get_handle(PipelineLayout layout) { return (!layout ? NULL : layout->handle); }
