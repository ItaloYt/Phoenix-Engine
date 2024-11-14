#ifndef PHOENIX_RENDERER_VULKAN_IMAGES_H
#define PHOENIX_RENDERER_VULKAN_IMAGES_H

#include "renderer/vulkan/swapchain.h"
#include "util/error.h"

typedef struct Images *Images;

Error images_create(Images *images, Swapchain swapchain);
Error images_recreate(Images images);
void images_destroy(Images images);

Swapchain images_get_swapchain(Images images);
Device images_get_device(Images images);
const VkImage *images_get_handles(Images images);
unsigned images_get_length(Images images);

#endif
