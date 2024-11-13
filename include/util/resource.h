#ifndef PHOENIX_UTIL_RESOURCES_H
#define PHOENIX_UTIL_RESOURCES_H

#include "util/error.h"

typedef struct Resource *Resource;

typedef enum {
  TYPE_UNKNOWN = 0,
  TYPE_SPIRV = 1,
} ResourceType;

extern const unsigned resource_count;

Error resource_create(Resource *resource, ResourceType type, const char *name, const char *data, unsigned size);
void resource_destroy(Resource resource);

const char *resource_get_handle(Resource resource);
const char *resource_get_name(Resource resource);
unsigned resource_get_size(Resource resource);

Error resource_load(Resource *resources);

#endif
