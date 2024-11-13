#ifndef PHOENIX_RENDERER_VULKAN_IMAGE_VIEWS_H
#define PHOENIX_RENDERER_VULKAN_IMAGE_VIEWS_H

#include "renderer/vulkan/images.h"
#include "util/error.h"

typedef struct ImageViews *ImageViews;

Error image_views_create(ImageViews *views, Images images);
void image_views_destroy(ImageViews views);

Images image_views_get_images(ImageViews views);
Device image_views_get_device(ImageViews views);
const VkImageView *image_views_get_handles(ImageViews views);
unsigned image_views_get_length(ImageViews views);

#endif
