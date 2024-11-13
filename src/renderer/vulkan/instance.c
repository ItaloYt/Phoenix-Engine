#include "renderer/vulkan/instance.h"
#include "util/error.h"
#include "window.h"
#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan_core.h>

struct Instance {
  Window window;
  VkInstance handle;
};

static const char *debug_extensions[] = {
  VK_EXT_DEBUG_UTILS_EXTENSION_NAME
};
static const unsigned debug_extensions_length = sizeof(debug_extensions) / sizeof(debug_extensions[0]);

// TODO: User layers
// TODO: User extensions
Error instance_create(Instance *instance, Window window, const char *appname, unsigned appversion) {
  if(!instance || !window || !appname) return NULL_HANDLE_ERROR;

  *instance = malloc(sizeof(struct Instance));
  if(!*instance) return ALLOCATION_ERROR;

  (*instance)->window = window;

  const char **required_extensions;
  unsigned required_extensions_length;

  if(window_get_extensions(window, &required_extensions, &required_extensions_length) != SUCCESS)
    return WINDOW_GET_EXTENSIONS_ERROR;

  unsigned extensions_length = required_extensions_length + debug_extensions_length;
  const char **extensions = malloc(extensions_length * sizeof(const char *));
  if(!extensions) return ALLOCATION_ERROR;

  memcpy(extensions, required_extensions, required_extensions_length * sizeof(const char *));
  memcpy(extensions + required_extensions_length, debug_extensions, debug_extensions_length * sizeof(const char *));

  VkInstanceCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .pApplicationInfo = &(VkApplicationInfo){
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pNext = NULL,
      .pApplicationName = appname,
      .applicationVersion = appversion,
      .pEngineName = "Phoenix",
      .engineVersion = VK_MAKE_API_VERSION(0, 0, 0, 0),
      .apiVersion = VK_API_VERSION_1_3,
    },
    .enabledLayerCount = 1,
    .ppEnabledLayerNames = (const char *[]){
      "VK_LAYER_KHRONOS_validation",
    },
    .enabledExtensionCount = extensions_length,
    .ppEnabledExtensionNames = extensions,
  };

  if(vkCreateInstance(&info, NULL, &(*instance)->handle) != VK_SUCCESS)
    return INSTANCE_CREATE_ERROR;

  free(extensions);

  return SUCCESS;
}

void instance_destroy(Instance instance) {
  if(!instance) return;

  vkDestroyInstance(instance->handle, NULL);

  free(instance);
}

Window instance_get_window(Instance instance) { return (!instance ? NULL : instance->window); }
VkInstance instance_get_handle(Instance instance) { return (!instance ? NULL : instance->handle); }
