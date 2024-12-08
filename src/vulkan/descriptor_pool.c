#include "vulkan/descriptor_pool.h"
#include "util/error.h"
#include <vulkan/vulkan_core.h>

Error vulkan_descriptor_pool_create(VkDescriptorPool *pool, VkDevice device, unsigned max_sets, const VkDescriptorPoolSize *sizes, unsigned length) {
    VkDescriptorPoolCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .maxSets = max_sets,
        .poolSizeCount = length,
        .pPoolSizes = sizes,
    };

    if(vkCreateDescriptorPool(device, &info, NULL, pool) != VK_SUCCESS)
        return VULKAN_DESCRIPTOR_POOL_CREATE_ERROR;

    return SUCCESS;
}
