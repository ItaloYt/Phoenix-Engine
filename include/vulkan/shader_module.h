#ifndef PHOENIX_VULKAN_SHADER_MODULE_H
#define PHOENIX_VULKAN_SHADER_MODULE_H

#include "util/error.h"
#include <vulkan/vulkan_core.h>

Error vulkan_shader_module_create(VkShaderModule *module, VkDevice device, const void *data, unsigned size);

#endif
