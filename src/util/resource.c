#include "util/resource.h"
#include "util/error.h"
#include <stdlib.h>
#include <string.h>

struct Resource {
  char *handle;
  char *name;
  ResourceType type;
  unsigned size;
};

Error resource_create(Resource *resource, ResourceType type, const char *name, const char *data, unsigned size) {
  if(!resource) return NULL_HANDLE_ERROR;

  *resource = malloc(sizeof(struct Resource));
  if(!*resource) return ALLOCATION_ERROR;

  (*resource)->type = type;

  (*resource)->size = size;
  (*resource)->handle = malloc(size * sizeof(char));
  if(!(*resource)->handle) return ALLOCATION_ERROR;
  (void)memcpy((*resource)->handle, data, size * sizeof(char));

  unsigned name_length = strlen(name);
  (*resource)->name = malloc((name_length + 1) * sizeof(char));
  if(!(*resource)->name) return ALLOCATION_ERROR;
  (void)memcpy((*resource)->name, name, (name_length + 1) * sizeof(char));

  return SUCCESS;
}

void resource_destroy(Resource resource) {
  if(!resource) return;

  free(resource->handle);
  free(resource->name);
  free(resource);
}

const char *resource_get_handle(Resource resource) { return (!resource ? NULL : resource->handle); }
const char *resource_get_name(Resource resource) { return (!resource ? NULL : resource->name); }
ResourceType resource_get_type(Resource resource) { return (!resource ? TYPE_UNKNOWN : resource->type); }
unsigned resource_get_size(Resource resource) { return (!resource ? 0 : resource->size); }
