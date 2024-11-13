#include "renderer/vulkan/shader_module.h"
#include "renderer/vulkan/device.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

struct ShaderModule {
  Device device;
  VkShaderModule handle;
};

Error shader_module_create(ShaderModule *module, Device device, const void *data, unsigned size) {
  if(!module || !device || !data) return NULL_HANDLE_ERROR;

  const VkDevice vk_device = device_get_handle(device);

  *module = malloc(sizeof(struct ShaderModule));
  if(!*module) return ALLOCATION_ERROR;

  (*module)->device = device;

  VkShaderModuleCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .codeSize = size,
    .pCode = data,
  };

  if(vkCreateShaderModule(vk_device, &info, NULL, &(*module)->handle) != VK_SUCCESS)
    return SHADER_MODULE_CREATE_ERROR;

  return SUCCESS;
}

void shader_module_destroy(ShaderModule module) {
  if(!module) return;

  const VkDevice vk_device = device_get_handle(module->device);

  vkDestroyShaderModule(vk_device, module->handle, NULL);

  free(module);
}

VkShaderModule shader_module_get_handle(ShaderModule module) { return (!module ? NULL : module->handle); }
Device shader_module_get_device(ShaderModule module) { return (!module ? NULL : module->device); }
