#ifndef PHOENIX_RENDERER_VULKAN_SHADER_MODULE_H
#define PHOENIX_RENDERER_VULKAN_SHADER_MODULE_H

#include "renderer/vulkan/device.h"
#include <vulkan/vulkan_core.h>

typedef struct ShaderModule *ShaderModule;

Error shader_module_create(ShaderModule *module, Device device, const void *data, unsigned size);
void shader_module_destroy(ShaderModule module);

VkShaderModule shader_module_get_handle(ShaderModule module);
Device shader_module_get_device(ShaderModule module);

#endif
