#ifndef PHOENIX_RENDERER_VULKAN_INSTANCE_H
#define PHOENIX_RENDERER_VULKAN_INSTANCE_H

#include "util/error.h"
#include "window.h"

typedef struct Instance *Instance;

Error instance_create(Instance *instance, Window window, const char *appname, unsigned appversion);
void instance_destroy(Instance instance);

Window instance_get_window(Instance instance);
VkInstance instance_get_handle(Instance instance);

#endif
