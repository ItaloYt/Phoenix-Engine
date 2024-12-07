#ifndef PHOENIX_UTIL_RESOURCES_H
#define PHOENIX_UTIL_RESOURCES_H

#include "util/error.h"

typedef enum {
  TYPE_UNKNOWN = 0,
  TYPE_SPIRV = 1,
} ResourceType;

typedef struct Resource {
  ResourceType type;
  const char *name;
  unsigned size;
  char *handle;
} Resource;

extern const unsigned resources_length;
extern Resource resources[];

Error resource_create(Resource *resource, const char *data);
void resource_destroy(const Resource *resource);

Error resources_load();
void resources_destroy();

#endif
