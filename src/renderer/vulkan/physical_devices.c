#include "renderer/vulkan/physical_devices.h"
#include "renderer/vulkan/instance.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

struct PhysicalDevices {
  Instance instance;
  VkPhysicalDevice *handles;
  unsigned length;
};

Error physical_devices_create(PhysicalDevices *devices, Instance instance) {
  if(!devices || !instance) return NULL_HANDLE_ERROR;

  const VkInstance vk_instance = instance_get_handle(instance);

  *devices = malloc(sizeof(struct PhysicalDevices));
  if(!*devices) return ALLOCATION_ERROR;

  (*devices)->instance = instance;

  if(vkEnumeratePhysicalDevices(vk_instance, &(*devices)->length, NULL) != VK_SUCCESS)
    return PHYSICALS_ENUMERATE_ERROR;

  (*devices)->handles = malloc((*devices)->length * sizeof(VkPhysicalDevice));
  if(!(*devices)->handles) return ALLOCATION_ERROR;

  if(vkEnumeratePhysicalDevices(vk_instance, &(*devices)->length, (*devices)->handles) != VK_SUCCESS)
    return PHYSICALS_ENUMERATE_ERROR;

  return SUCCESS;
}

void physical_devices_destroy(PhysicalDevices devices) {
  if(!devices) return;

  free(devices->handles);

  free(devices);
}

Instance physical_devices_get_instance(PhysicalDevices devices) { return (!devices ? NULL : devices->instance); }
VkPhysicalDevice *physical_devices_get_handles(PhysicalDevices devices) { return (!devices ? NULL : devices->handles); }
unsigned physical_devices_get_length(PhysicalDevices devices) { return (!devices ? 0 : devices->length); }
