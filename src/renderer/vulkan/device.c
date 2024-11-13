#include "renderer/vulkan/device.h"
#include "renderer/vulkan/surface.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

struct Device {
  Surface surface;
  VkPhysicalDevice physical;
  VkDevice handle;
  VkSurfaceFormatKHR *formats;
  VkPresentModeKHR *modes;
  VkSurfaceCapabilitiesKHR capabilities;
  VkPhysicalDeviceProperties properties;
  VkPhysicalDeviceFeatures features;
  unsigned graphic_family;
  unsigned present_family;
  unsigned formats_length;
  unsigned modes_length;
};

// TODO: User device extensions
// TODO: User features
Error device_create(Device *device, Surface surface, VkPhysicalDevice physical) {
  if(!device || !surface || !physical) return NULL_HANDLE_ERROR;

  const VkSurfaceKHR vk_surface = surface_get_handle(surface);

  *device = malloc(sizeof(struct Device));
  if(!*device) return ALLOCATION_ERROR;

  (*device)->physical = physical;
  (*device)->surface = surface;
  (*device)->graphic_family = -1;
  (*device)->present_family = -1;

  vkGetPhysicalDeviceProperties(physical, &(*device)->properties);
  vkGetPhysicalDeviceFeatures(physical, &(*device)->features);

  if(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical, vk_surface, &(*device)->capabilities) != VK_SUCCESS)
    return DEVICE_GET_CAPABILITIES_ERROR;

  if(vkGetPhysicalDeviceSurfaceFormatsKHR(physical, vk_surface, &(*device)->formats_length, NULL) != VK_SUCCESS)
    return DEVICE_GET_FORMATS_ERROR;

  (*device)->formats = malloc((*device)->formats_length * sizeof(VkSurfaceFormatKHR));
  if(!(*device)->formats) return ALLOCATION_ERROR;

  if(vkGetPhysicalDeviceSurfaceFormatsKHR(physical, vk_surface, &(*device)->formats_length, (*device)->formats) != VK_SUCCESS)
    return DEVICE_GET_FORMATS_ERROR;

  if(vkGetPhysicalDeviceSurfacePresentModesKHR(physical, vk_surface, &(*device)->modes_length, NULL) != VK_SUCCESS)
    return DEVICE_GET_PRESENT_MODES_ERROR;

  (*device)->modes = malloc((*device)->modes_length * sizeof(VkPresentModeKHR));
  if(!(*device)->modes) return ALLOCATION_ERROR;

  if(vkGetPhysicalDeviceSurfacePresentModesKHR(physical, vk_surface, &(*device)->modes_length, (*device)->modes) != VK_SUCCESS)
    return DEVICE_GET_PRESENT_MODES_ERROR;

  unsigned families_length = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physical, &families_length, NULL);

  VkQueueFamilyProperties families[families_length];
  vkGetPhysicalDeviceQueueFamilyProperties(physical, &families_length, families);

  for(unsigned index = 0; index < families_length && ((*device)->graphic_family == -1 || (*device)->present_family == -1); ++index) {
    if((*device)->graphic_family == -1 && families[index].queueFlags & VK_QUEUE_GRAPHICS_BIT)
      (*device)->graphic_family = index;

    VkBool32 is_present_available = VK_FALSE;
    if(vkGetPhysicalDeviceSurfaceSupportKHR(physical, index, vk_surface, &is_present_available) != VK_SUCCESS)
      return DEVICE_GET_SURFACE_SUPPORT_ERROR;

    if(!is_present_available) continue;

    (*device)->present_family = index;
  }

  if((*device)->graphic_family == -1) return DEVICE_NO_GRAPHIC_QUEUE_FAMILY_ERROR;
  if((*device)->present_family == -1) return DEVICE_NO_PRESENT_QUEUE_FAMILY_ERROR;

  VkDeviceCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .queueCreateInfoCount = 2,
    .pQueueCreateInfos = (VkDeviceQueueCreateInfo[]){
      (VkDeviceQueueCreateInfo){
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .queueFamilyIndex = (*device)->graphic_family,
        .queueCount = 1,
        .pQueuePriorities = &(float){ 1 },
      },
      (VkDeviceQueueCreateInfo){
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .queueFamilyIndex = (*device)->present_family,
        .queueCount = 1,
        .pQueuePriorities = &(float){ 1 },
      },
    },
    .enabledLayerCount = 0,
    .ppEnabledLayerNames = NULL,
    .enabledExtensionCount = 1,
    .ppEnabledExtensionNames = (const char *[]){
      VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    },
    .pEnabledFeatures = &(VkPhysicalDeviceFeatures){ 0 },
  };

  if((*device)->graphic_family == (*device)->present_family) {
    info.queueCreateInfoCount = 1;
  }

  if(vkCreateDevice(physical, &info, NULL, &(*device)->handle) != VK_SUCCESS)
    return DEVICE_CREATE_ERROR;

  return SUCCESS;
}

void device_destroy(Device device) {
  if(!device) return;

  vkDestroyDevice(device->handle, NULL);
  free(device->modes);
  free(device->formats);
  free(device);
}

Surface device_get_surface(Device device) { return (!device ? NULL : device->surface); };
VkPhysicalDevice device_get_physical_device(Device device) { return (!device ? NULL : device->physical); };
VkDevice device_get_handle(Device device) { return (!device ? NULL : device->handle); };
const VkSurfaceCapabilitiesKHR *device_get_capabilities(Device device) { return (!device ? NULL : &device->capabilities); }
const VkSurfaceFormatKHR *device_get_surface_formats(Device device) { return (!device ? NULL : device->formats); }
const VkPresentModeKHR *device_get_surface_present_modes(Device device) { return (!device ? NULL : device->modes); }
unsigned device_get_graphic_family_index(Device device) { return (!device ? -1 : device->graphic_family); };
unsigned device_get_present_family_index(Device device) { return (!device ? -1 : device->present_family); };
unsigned device_get_surface_formats_length(Device device) { return (!device ? 0 : device->formats_length); }
unsigned device_get_surface_present_modes_length(Device device) { return (!device ? 0 : device->modes_length); }
