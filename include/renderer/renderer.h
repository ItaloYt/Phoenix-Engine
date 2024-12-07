#ifndef PHOENIX_RENDERER_H
#define PHOENIX_RENDERER_H

#include "renderer/vulkan/command_buffer.h"
#include "renderer/vulkan/command_pool.h"
#include "renderer/vulkan/device.h"
#include "renderer/vulkan/fence.h"
#include "renderer/vulkan/image_views.h"
#include "renderer/vulkan/images.h"
#include "renderer/vulkan/instance.h"
#include "renderer/vulkan/physical_devices.h"
#include "renderer/vulkan/queue.h"
#include "renderer/vulkan/semaphore.h"
#include "renderer/vulkan/surface.h"
#include "renderer/vulkan/swapchain.h"
#include "util/error.h"
#include <vulkan/vulkan_core.h>

#define MAX_RENDERING_FRAMES 2

typedef struct Renderer Renderer;

typedef struct Renderer {
  Error (*select_physical)(const VulkanPhysicalDevices *physicals, unsigned *index);
  Error (*select_details)(const VulkanDevice *device, unsigned *format, unsigned *mode);
  Error (*create_surface)(void *user, VulkanSurface *surface);
  Error (*draw)(const Renderer *renderer);
} Renderer;

Error renderer_create(Renderer *renderer, void *user, const char *appname, unsigned appversion);
Error renderer_draw(Renderer *renderer);
Error renderer_resize(Renderer *renderer);
void renderer_destroy(const Renderer *renderer);

// Error renderer_draw_callback(Renderer renderer, void *data, CommandBuffer buffer, unsigned image_index);

#endif
