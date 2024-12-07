#include "vulkan/instance.h"
#include "util/error.h"
#include <vulkan/vulkan_core.h>

static const char *debug[] = {
  VK_EXT_DEBUG_UTILS_EXTENSION_NAME
};
static const unsigned debug_length = sizeof(debug) / sizeof(debug[0]);

// TODO: User layers
// TODO: User extensions
Error vulkan_instance_create(VkInstance *instance, const char *name, unsigned version, const char *const *extensions, unsigned length) {
  unsigned total_length = length + debug_length;
  const char *ext[total_length];

  unsigned index = 0;
  for(; index < length; ++index)
    ext[index] = extensions[index];

  for(; index < debug_length + length; ++index)
    ext[index] = debug[index - length];

  VkInstanceCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .pApplicationInfo = &(VkApplicationInfo){
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pNext = NULL,
      .pApplicationName = name,
      .applicationVersion = version,
      .pEngineName = "Phoenix",
      .engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
      .apiVersion = VK_API_VERSION_1_3,
    },
    .enabledLayerCount = 1,
    .ppEnabledLayerNames = (const char *[]){
      "VK_LAYER_KHRONOS_validation",
    },
    .enabledExtensionCount = total_length,
    .ppEnabledExtensionNames = ext,
  };

  if(vkCreateInstance(&info, NULL, instance) != VK_SUCCESS)
    return VULKAN_INSTANCE_CREATE_ERROR;

  return SUCCESS;
}
