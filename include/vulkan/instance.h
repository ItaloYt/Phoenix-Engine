#ifndef PHOENIX_VULKAN_INSTANCE_H
#define PHOENIX_VULKAN_INSTANCE_H

#include "util/error.h"
#include <vulkan/vulkan_core.h>

Error vulkan_instance_create(VkInstance *instance, const char *name, unsigned version, const char *const *extensions, unsigned length);

#endif
