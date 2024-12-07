#include "vulkan/device.h"
#include "util/error.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

// TODO: User device extensions
// TODO: User features
Error vulkan_device_create(VkDevice *device, VkPhysicalDevice physical, unsigned *indices, unsigned *count, float **priorities, unsigned length) {
  VkDeviceQueueCreateInfo queues[length];
  for(unsigned index = 0; index < length; ++index) {
    queues[index] = (VkDeviceQueueCreateInfo){
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .queueFamilyIndex = indices[index],
      .queueCount = count[index],
      .pQueuePriorities = priorities[index],
    };
  }

  VkDeviceCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .queueCreateInfoCount = length,
    .pQueueCreateInfos = queues,
    .enabledLayerCount = 0,
    .ppEnabledLayerNames = NULL,
    .enabledExtensionCount = 1,
    .ppEnabledExtensionNames = (const char *[]){
      VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    },
    .pEnabledFeatures = &(VkPhysicalDeviceFeatures){ 0 },
  };

  if(vkCreateDevice(physical, &info, NULL, device) != VK_SUCCESS)
    return VULKAN_DEVICE_CREATE_ERROR;

  return SUCCESS;
}
