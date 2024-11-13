#ifndef PHOENIX_RENDERER_VULKAN_DEVICE_H
#define PHOENIX_RENDERER_VULKAN_DEVICE_H

#include "renderer/vulkan/surface.h"
#include "util/error.h"
#include <vulkan/vulkan_core.h>

typedef struct Device *Device;

Error device_create(Device *device, Surface surface, VkPhysicalDevice physical);
void device_destroy(Device device);

Surface device_get_surface(Device device);
VkPhysicalDevice device_get_physical_device(Device device);
VkDevice device_get_handle(Device device);
const VkSurfaceCapabilitiesKHR *device_get_capabilities(Device device);
const VkSurfaceFormatKHR *device_get_surface_formats(Device device);
const VkPresentModeKHR *device_get_surface_present_modes(Device device);
unsigned device_get_graphic_family_index(Device device);
unsigned device_get_present_family_index(Device device);
unsigned device_get_surface_formats_length(Device device);
unsigned device_get_surface_present_modes_length(Device device);

#endif
