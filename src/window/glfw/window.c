#include "window/window.h"
#include "util/error.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

struct ResizeCallback {
  void *user;
  WindowResize resize;
};

static void window_resized(GLFWwindow *window, int width, int height);

Error window_start_context() {
  if(!glfwInit()) return WINDOW_START_CONTEXT_ERROR;

  return SUCCESS;
}

Error window_create(Window *window, const char *title, unsigned width, unsigned height, WindowResize resize, void *user) {
  if(!window) return NULL_HANDLE_ERROR;

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  *window = (Window)glfwCreateWindow(width, height, title, NULL, NULL);
  if(!*window) return WINDOW_CREATE_ERROR;

  struct ResizeCallback *window_user = glfwGetWindowUserPointer((GLFWwindow *)*window);

  if(!window_user) {
    window_user = malloc(sizeof(struct ResizeCallback));
    if(!window_user) return ALLOCATION_ERROR;
  }

  window_user->user = user;
  window_user->resize = resize;

  glfwSetWindowUserPointer((GLFWwindow *)*window, window_user);
  glfwSetFramebufferSizeCallback((GLFWwindow *)*window, window_resized);

  return SUCCESS;
}

Error window_create_vulkan_surface(Window *window, VkInstance instance, VkSurfaceKHR *surface) {
  if(glfwCreateWindowSurface(instance, (GLFWwindow *)*window, NULL, surface) != VK_SUCCESS)
    return VULKAN_SURFACE_CREATE_ERROR;

  return SUCCESS;
}

void window_update(Window *window) {
  glfwPollEvents();
}

void window_destroy(Window *window) {
  glfwDestroyWindow((GLFWwindow *)*window);
}

void window_end_context() {
  glfwTerminate();
}

void window_get_extensions(const char ***extensions, unsigned *length) {
  const char **tmp = glfwGetRequiredInstanceExtensions(length);
  if(extensions) *extensions = tmp;
}

int window_is_closed(Window *window) {
  return (!window ? 0 : glfwWindowShouldClose((GLFWwindow *)*window));
}

void window_get_size(Window *window, int *width, int *height) {
  glfwGetFramebufferSize((GLFWwindow *)*window, width, height);
}

static void window_resized(GLFWwindow *window, int width, int height) {
  struct ResizeCallback *window_user = glfwGetWindowUserPointer(window);

  if(window_user->resize) window_user->resize(window_user->user, width, height);
}
