#ifndef PHOENIX_RENDERER_VULKAN_SURFACE_H
#define PHOENIX_RENDERER_VULKAN_SURFACE_H

#include "renderer/vulkan/instance.h"
#include "util/error.h"

typedef struct Surface *Surface;

Error surface_create(Surface *surface, Instance instance);
void surface_destroy(Surface surface);

Instance surface_get_instance(Surface surface);
VkSurfaceKHR surface_get_handle(Surface surface);

#endif
