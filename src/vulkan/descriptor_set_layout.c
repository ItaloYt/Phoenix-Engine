#include "vulkan/descriptor_set_layout.h"
#include "util/error.h"
#include <vulkan/vulkan_core.h>

Error vulkan_descriptor_set_layout_create(VkDescriptorSetLayout *layout, VkDevice device, const VkDescriptorSetLayoutBinding *bindings, unsigned length) {
    VkDescriptorSetLayoutCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .bindingCount = length,
        .pBindings = bindings,
    };

    if(vkCreateDescriptorSetLayout(device, &info, NULL, layout) != VK_SUCCESS)
        return VULKAN_DESCRIPTOR_SET_LAYOUT_CREATE_ERROR;

    return SUCCESS;
}
