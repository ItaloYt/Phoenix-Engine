#include "vulkan/image_views.h"
#include "util/error.h"
#include "vulkan/images.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

Error vulkan_image_views_create(VulkanImageViews *views, VkDevice device, VkFormat format, const VulkanImages *images) {
  if(!views || !images) return NULL_HANDLE_ERROR;

  views->length = images->length;

  void *tmp = realloc(views->handles, views->length * sizeof(VkImageView));
  if(!tmp) return ALLOCATION_ERROR;
  views->handles = tmp;

  VkImageViewCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .image = NULL, // Will be set later
    .viewType = VK_IMAGE_VIEW_TYPE_2D,
    .format = format,
    .components = {
      .r = VK_COMPONENT_SWIZZLE_IDENTITY,
      .g = VK_COMPONENT_SWIZZLE_IDENTITY,
      .b = VK_COMPONENT_SWIZZLE_IDENTITY,
      .a = VK_COMPONENT_SWIZZLE_IDENTITY,
    },
    .subresourceRange = {
      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      .baseMipLevel = 0,
      .levelCount = 1,
      .baseArrayLayer = 0,
      .layerCount = 1,
    },
  };

  for(unsigned index = 0; index < views->length; ++index) {
    info.image = images->handles[index];

    if(vkCreateImageView(device, &info, NULL, views->handles + index) != VK_SUCCESS)
      return VULKAN_IMAGE_VIEWS_CREATE_ERROR;
  }

  return SUCCESS;
}

void vulkan_image_views_destroy(VulkanImageViews *views, VkDevice device) {
  if(!views) return;

  for(unsigned index = 0; index < views->length; ++index)
    if(views->handles[index])
      vkDestroyImageView(device, views->handles[index], NULL);

  free(views->handles);
  views->handles = 0x0;
}
