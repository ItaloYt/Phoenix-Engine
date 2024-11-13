#ifndef PHOENIX_RENDERER_VULKAN_FENCE_H
#define PHOENIX_RENDERER_VULKAN_FENCE_H

#include "renderer/vulkan/device.h"
#include "util/error.h"
#include <vulkan/vulkan_core.h>

typedef struct Fence *Fence;

Error fence_create(Fence *fence, Device device);
Error fences_wait(Fence *fences, unsigned length);
Error fences_reset(Fence *fences, unsigned length);
void fence_destroy(Fence fence);

Device fence_get_device(Fence fence);
VkFence fence_get_handle(Fence fence);

#endif
