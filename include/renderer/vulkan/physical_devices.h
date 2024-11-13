#ifndef PHOENIX_RENDERER_VULKAN_PHYSICAL_DEVICES_H
#define PHOENIX_RENDERER_VULKAN_PHYSICAL_DEVICES_H

#include "renderer/vulkan/instance.h"
#include "util/error.h"

typedef struct PhysicalDevices *PhysicalDevices;

Error physical_devices_create(PhysicalDevices *devices, Instance instance);
void physical_devices_destroy(PhysicalDevices devices);

Instance physical_devices_get_instance(PhysicalDevices devices);
VkPhysicalDevice *physical_devices_get_handles(PhysicalDevices devices);
unsigned physical_devices_get_length(PhysicalDevices devices);

#endif
