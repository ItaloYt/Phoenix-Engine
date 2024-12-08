#include "vulkan/descriptor_set.h"
#include "util/error.h"
#include <vulkan/vulkan_core.h>

Error vulkan_descriptor_set_create(VkDescriptorSet *sets, VkDevice device, VkDescriptorPool pool, VkDescriptorSetLayout *layouts, unsigned length) {
    VkDescriptorSetAllocateInfo info = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = NULL,
        .descriptorPool = pool,
        .descriptorSetCount = length,
        .pSetLayouts = layouts,
    };

    if(vkAllocateDescriptorSets(device, &info, sets) != VK_SUCCESS)
        return VULKAN_DESCRIPTOR_SET_ALLOCATE_ERROR;

    return SUCCESS;
}
