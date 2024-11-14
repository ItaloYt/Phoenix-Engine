#ifndef PHOENIX_WINDOW_H
#define PHOENIX_WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "util/error.h"

typedef struct Window *Window;

Error window_create(Window *window, void *context, const char *title, unsigned width, unsigned height);
void window_update(Window window);
void window_destroy(Window window);

GLFWwindow *window_get_handle(Window window);
Error window_get_extensions(Window window, const char ***extensions, unsigned *length);
int window_is_closed(Window window);
void window_get_framebuffer_size(Window window, unsigned *width, unsigned *height);

void window_resize_callback(void *handle, int width, int height);

#endif
