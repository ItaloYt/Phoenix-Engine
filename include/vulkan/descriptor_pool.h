#ifndef PHOENIX_VULKAN_DESCRIPTOR_POOL_H
#define PHOENIX_VULKAN_DESCRIPTOR_POOL_H

#include "util/error.h"
#include <vulkan/vulkan_core.h>

Error vulkan_descriptor_pool_create(VkDescriptorPool *pool, VkDevice device, unsigned max_sets, const VkDescriptorPoolSize *sizes, unsigned length);

#endif
