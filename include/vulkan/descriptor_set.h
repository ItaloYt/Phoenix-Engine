#ifndef PHOENIX_VULKAN_DESCRIPTOR_SET_H
#define PHOENIX_VULKAN_DESCRIPTOR_SET_H

#include "util/error.h"
#include <vulkan/vulkan_core.h>

Error vulkan_descriptor_set_create(VkDescriptorSet *sets, VkDevice device, VkDescriptorPool pool, VkDescriptorSetLayout *layouts, unsigned length);

#endif
