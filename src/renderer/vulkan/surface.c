#include "renderer/vulkan/surface.h"
#include "renderer/vulkan/instance.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

struct Surface {
  Instance instance;
  VkSurfaceKHR handle;
};

// Private Function
Error surface_handle_create(VkSurfaceKHR *handle, Instance instance);

Error surface_create(Surface *surface, Instance instance) {
  if(!surface || !instance) return NULL_HANDLE_ERROR;

  *surface = malloc(sizeof(struct Surface));
  if(!*surface) return ALLOCATION_ERROR;

  (*surface)->instance = instance;

  return surface_handle_create(&(*surface)->handle, instance);
}

void surface_destroy(Surface surface) {
  if(!surface) return;

  const VkInstance vk_instance = instance_get_handle(surface->instance);

  vkDestroySurfaceKHR(vk_instance, surface->handle, NULL);

  free(surface);
}

Instance surface_get_instance(Surface surface) { return (!surface ? NULL : surface->instance); }
VkSurfaceKHR surface_get_handle(Surface surface) { return (!surface ? NULL : surface->handle); }
