#include "renderer/vulkan/image_views.h"
#include "renderer/vulkan/device.h"
#include "renderer/vulkan/images.h"
#include "renderer/vulkan/swapchain.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

struct ImageViews {
  Images images;
  Device device;
  VkImageView *handles;
  unsigned length;
};

Error image_views_create(ImageViews *views, Images images) {
  if(!views || !images) return NULL_HANDLE_ERROR;

  const Device device = images_get_device(images);
  const VkDevice vk_device = device_get_handle(device);
  const VkImage *vk_images = images_get_handles(images);
  const Swapchain swapchain = images_get_swapchain(images);
  const VkSurfaceFormatKHR *format = swapchain_get_surface_format(swapchain);

  *views = malloc(sizeof(struct ImageViews));
  if(!*views) return ALLOCATION_ERROR;

  (*views)->images = images;
  (*views)->device = device;
  (*views)->length = images_get_length(images);

  (*views)->handles = malloc((*views)->length * sizeof(VkImageView));
  if(!(*views)->handles) return ALLOCATION_ERROR;

  VkImageViewCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .image = NULL,
    .viewType = VK_IMAGE_VIEW_TYPE_2D,
    .format = format->format,
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

  for(unsigned index = 0; index < (*views)->length; ++index) {
    info.image = vk_images[index];

    if(vkCreateImageView(vk_device, &info, NULL, (*views)->handles + index) != VK_SUCCESS)
      return IMAGE_VIEWS_CREATE_ERROR;
  }

  return SUCCESS;
}

void image_views_destroy(ImageViews views) {
  if(!views) return;

  const VkDevice vk_device = device_get_handle(views->device);

  for(unsigned index = 0; index < views->length; ++index)
    vkDestroyImageView(vk_device, views->handles[index], NULL);

  free(views->handles);
  free(views);
}

Images image_views_get_images(ImageViews views) { return (!views ? NULL : views->images); }
Device image_views_get_device(ImageViews views) { return (!views ? NULL : views->device); }
const VkImageView *image_views_get_handles(ImageViews views) { return (!views ? NULL : views->handles); }
unsigned image_views_get_length(ImageViews views) { return (!views ? 0 : views->length); }
