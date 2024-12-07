#include "vulkan/shader_module.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

Error vulkan_shader_module_create(VkShaderModule *module, VkDevice device, const void *data, unsigned size) {
  VkShaderModuleCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .codeSize = size,
    .pCode = data,
  };

  if(vkCreateShaderModule(device, &info, NULL, module) != VK_SUCCESS)
    return VULKAN_SHADER_MODULE_CREATE_ERROR;

  return SUCCESS;
}
