#ifndef PHOENIX_WINDOW_H
#define PHOENIX_WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include "util/error.h"

typedef struct Window *Window;
typedef void (*WindowResize)(void *user, int width, int height);

Error window_start_context();
Error window_create(Window *window, const char *title, unsigned width, unsigned height, WindowResize resize, void *user);
Error window_create_vulkan_surface(Window *window, VkInstance instance, VkSurfaceKHR *surface);
void window_update(Window *window);
void window_destroy(Window *window);
void window_end_context();

void window_get_extensions(const char ***extensions, unsigned *length);
int window_is_closed(Window *window);
void window_get_size(Window *window, int *width, int *height);

#endif
