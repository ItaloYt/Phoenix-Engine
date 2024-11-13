#include "window.h"
#include "renderer/vulkan/instance.h"
#include "util/error.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

struct Window {
  GLFWwindow *handle;
};

Error window_create(Window *window, const char *title, unsigned width, unsigned height) {
  if(!window || !title) return NULL_HANDLE_ERROR;

  if(!glfwInit()) return WINDOW_FRAMEWORK_ERROR;

  *window = malloc(sizeof(struct Window));
  if(!*window) return ALLOCATION_ERROR;

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  (*window)->handle = glfwCreateWindow(width, height, title, NULL, NULL);
  if(!(*window)->handle) return WINDOW_CREATE_ERROR;

  return SUCCESS;
}

// TODO: Check if window is NULL?
void window_update(Window window) {
  glfwPollEvents();
}

void window_destroy(Window window) {
  if(!window) return;

  glfwDestroyWindow(window->handle);

  free(window);

  glfwTerminate();
}

GLFWwindow *window_get_handle(Window window) { return (!window ? NULL : window->handle); }
Error window_get_extensions(Window window, const char ***extensions, unsigned *length) {
  if(!window || !extensions) return NULL_HANDLE_ERROR;

  *extensions = glfwGetRequiredInstanceExtensions(length);

  return SUCCESS;
}
int window_is_closed(Window window) { return (!window ? 1 : glfwWindowShouldClose(window->handle)); }
void window_get_framebuffer_size(Window window, unsigned *width, unsigned *height) {
  if(!window) return;

  glfwGetFramebufferSize(window->handle, (int *)width, (int *)height);
}

Error surface_handle_create(VkSurfaceKHR *handle, Instance instance) {
  if(!handle || !instance) return NULL_HANDLE_ERROR;

  const Window window = instance_get_window(instance);
  const VkInstance vk_instance = instance_get_handle(instance);

  if(glfwCreateWindowSurface(vk_instance, window->handle, NULL, handle) != VK_SUCCESS)
    return SURFACE_CREATE_ERROR;

  return SUCCESS;
}
