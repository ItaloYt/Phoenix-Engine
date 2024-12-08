#ifndef PHOENIX_VULKAN_DESCRIPTOR_SET_LAYOUT_H
#define PHOENIX_VULKAN_DESCRIPTOR_SET_LAYOUT_H

#include "util/error.h"
#include <vulkan/vulkan_core.h>

Error vulkan_descriptor_set_layout_create(VkDescriptorSetLayout *layout, VkDevice device, const VkDescriptorSetLayoutBinding *bindings, unsigned length);

#endif
