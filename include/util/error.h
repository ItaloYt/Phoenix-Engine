#ifndef PHOENIX_UTIL_ERROR_H
#define PHOENIX_UTIL_ERROR_H

enum Error {
  // General Codes
  SUCCESS = 0,
  GENERAL_ERROR = 0x01000000,
  WINDOW_ERROR = 0x02000000,
  VULKAN_ERROR = 0x03000000,
  UTIL_ERROR = 0x04000000,
  RENDERER_ERROR = 0x05000000,

  // Specific Codes
  // General
  NULL_HANDLE_ERROR = GENERAL_ERROR | 0x01,
  ALLOCATION_ERROR = GENERAL_ERROR | 0x02,
  UNKNOWN_ERROR = GENERAL_ERROR | 0x03,

  // Window
  WINDOW_CREATE_ERROR = WINDOW_ERROR | 0x01,
  WINDOW_FRAMEWORK_ERROR = WINDOW_ERROR | 0x02,
  WINDOW_GET_EXTENSIONS_ERROR = WINDOW_ERROR | 0x03,

  // Vulkan
  INSTANCE_ERROR = VULKAN_ERROR | 0x010000,
  INSTANCE_CREATE_ERROR = INSTANCE_ERROR | 0x01,

  SURFACE_ERROR = VULKAN_ERROR | 0x020000,
  SURFACE_CREATE_ERROR = SURFACE_ERROR | 0x01,

  PHYSICALS_ERROR = VULKAN_ERROR | 0x030000,
  PHYSICALS_ENUMERATE_ERROR = PHYSICALS_ERROR | 0x01,

  DEVICE_ERROR = VULKAN_ERROR | 0x040000,
  DEVICE_NO_GRAPHIC_QUEUE_FAMILY_ERROR = DEVICE_ERROR | 0x01,
  DEVICE_CREATE_ERROR = DEVICE_ERROR | 0x02,
  DEVICE_GET_SURFACE_SUPPORT_ERROR = DEVICE_ERROR | 0x03,
  DEVICE_NO_PRESENT_QUEUE_FAMILY_ERROR = DEVICE_ERROR | 0x04,
  DEVICE_GET_CAPABILITIES_ERROR = DEVICE_ERROR | 0x05,
  DEVICE_GET_FORMATS_ERROR = DEVICE_ERROR | 0x06,
  DEVICE_GET_PRESENT_MODES_ERROR = DEVICE_ERROR | 0x07,
  DEVICE_RELOAD_SURFACE_ERROR = DEVICE_ERROR | 0x08,
  DEVICE_WAIT_IDLE_ERROR = DEVICE_ERROR | 0x09,

  SWAPCHAIN_ERROR = VULKAN_ERROR | 0x050000,
  SWAPCHAIN_CREATE_ERROR = SWAPCHAIN_ERROR | 0x01,
  SWAPCHAIN_ACQUIRE_IMAGE_ERROR = SWAPCHAIN_ERROR | 0x02,
  SWAPCHAIN_OUT_OF_DATE_ERROR = SWAPCHAIN_ERROR | 0x03,
  SWAPCHAIN_RECREATE_ERROR = SWAPCHAIN_ERROR | 0x04,

  IMAGES_ERROR = VULKAN_ERROR | 0x060000,
  IMAGES_GET_ERROR = IMAGES_ERROR | 0x01,
  IMAGES_RECREATE_ERROR = IMAGES_ERROR | 0x02,

  IMAGE_VIEWS_ERROR = VULKAN_ERROR | 0x070000,
  IMAGE_VIEWS_CREATE_ERROR = IMAGE_VIEWS_ERROR | 0x01,
  IMAGE_VIEWS_RECREATE_ERROR = IMAGE_VIEWS_ERROR | 0x02,

  RENDER_PASS_ERROR = VULKAN_ERROR | 0x080000,
  RENDER_PASS_CREATE_ERROR = RENDER_PASS_ERROR | 0x01,

  SHADER_MODULE_ERROR = VULKAN_ERROR | 0x090000,
  SHADER_MODULE_CREATE_ERROR = SHADER_MODULE_ERROR | 0x01,

  PIPELINE_LAYOUT_ERROR = VULKAN_ERROR | 0x0a0000,
  PIPELINE_LAYOUT_CREATE_ERROR = PIPELINE_LAYOUT_ERROR | 0x01,

  PIPELINE_ERROR = VULKAN_ERROR | 0x0b0000,
  PIPELINE_CREATE_ERROR = PIPELINE_ERROR | 0x01,

  FRAMEBUFFER_ERROR = VULKAN_ERROR | 0x0c0000,
  FRAMEBUFFER_CREATE_ERROR = FRAMEBUFFER_ERROR | 0x01,
  FRAMEBUFFERS_RECREATE_ERROR = FRAMEBUFFER_ERROR | 0x02,

  COMMAND_POOL_ERROR = VULKAN_ERROR | 0x0d0000,
  COMMAND_POOL_CREATE_ERROR = COMMAND_POOL_ERROR | 0x01,

  COMMAND_BUFFER_ERROR = VULKAN_ERROR | 0x0e0000,
  COMMAND_BUFFER_CREATE_ERROR = COMMAND_BUFFER_ERROR | 0x01,
  COMMAND_BUFFER_BEGIN_ERROR = COMMAND_BUFFER_ERROR | 0x02,
  COMMAND_BUFFER_RESET_ERROR = COMMAND_BUFFER_ERROR | 0x03,
  COMMAND_BUFFER_END_ERROR = COMMAND_BUFFER_ERROR | 0x04,

  SEMAPHORE_ERROR = VULKAN_ERROR | 0x0f0000,
  SEMAPHORE_CREATE_ERROR = SEMAPHORE_ERROR | 0x01,

  FENCE_ERROR = VULKAN_ERROR | 0x100000,
  FENCE_CREATE_ERROR = FENCE_ERROR | 0x01,
  FENCES_WAIT_ERROR = FENCE_ERROR | 0x02,
  FENCES_RESET_ERROR = FENCE_ERROR | 0x03,

  QUEUE_ERROR = VULKAN_ERROR | 0x110000,
  QUEUE_SUBMIT_ERROR = QUEUE_ERROR | 0x01,
  QUEUE_PRESENT_ERROR = QUEUE_ERROR | 0x02,

  // Utils
  FILE_ERROR = UTIL_ERROR | 0x010000,
  FILE_OPEN_ERROR = FILE_ERROR | 0x01,
  FILE_CLOSE_ERROR = FILE_ERROR | 0x02,
  FILE_SEEK_ERROR = FILE_ERROR | 0x03,
  FILE_READ_ERROR = FILE_ERROR | 0x04,
  FILE_INVALID_PERMISSION_ERROR = FILE_ERROR | 0x05,
  FILE_TRUNCATE_ERROR = FILE_ERROR | 0x06,
  FILE_WRITE_ERROR = FILE_ERROR | 0x07,

  RESOURCE_ERROR = UTIL_ERROR | 0x020000,
  RESOURCE_LOAD_ERROR = RESOURCE_ERROR | 0x01,
  RESOURCES_LOAD_ERROR = RESOURCE_ERROR | 0x02,

  // Renderer
  RENDERER_PHYSICALS_CREATE_ERROR = RENDERER_ERROR | 0x01,
  RENDERER_SELECT_PHYSICAL_ERROR = RENDERER_ERROR | 0x02,
  RENDERER_QUEUE_CREATE_ERROR = RENDERER_ERROR | 0x03,
  RENDERER_SELECT_SURFACE_FORMAT_ERROR = RENDERER_ERROR | 0x04,
  RENDERER_SELECT_SURFACE_PRESENT_MODE_ERROR = RENDERER_ERROR | 0x05,
  RENDERER_IMAGES_CREATE_ERROR = RENDERER_ERROR | 0x06,
  RENDERER_FRAMEBUFFERS_CREATE_ERROR = RENDERER_ERROR | 0x07,
  RENDERER_DRAW_ERROR = RENDERER_ERROR | 0x08,
  RENDERER_CREATE_ERROR = RENDERER_ERROR | 0x09,
};
typedef enum Error Error;

#endif
