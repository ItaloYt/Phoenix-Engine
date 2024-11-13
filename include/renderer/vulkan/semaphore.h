#ifndef PHOENIX_RENDERER_VULKAN_SEMAPHORE_H
#define PHOENIX_RENDERER_VULKAN_SEMAPHORE_H

#include "renderer/vulkan/device.h"
#include "util/error.h"
#include <vulkan/vulkan_core.h>

typedef struct Semaphore *Semaphore;

Error semaphore_create(Semaphore *semaphore, Device device);
void semaphore_destroy(Semaphore semaphore);

Device semaphore_get_device(Semaphore semaphore);
VkSemaphore semaphore_get_handle(Semaphore semaphore);

#endif
