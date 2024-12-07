#ifndef PHOENIX_VULKAN_RAW_BUFFER_H
#define PHOENIX_VULKAN_RAW_BUFFER_H

#include "util/error.h"
#include <vulkan/vulkan_core.h>

Error vulkan_raw_buffer_create(VkBuffer *buffer, VkDevice device, unsigned size, VkBufferUsageFlags usage, VkSharingMode sharing, unsigned *queues, unsigned length);

#endif
