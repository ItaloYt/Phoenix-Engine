#include "util/resource.h"
#include "util/error.h"
#include <stdlib.h>
#include <string.h>

Error resource_create(Resource *resource, const char *data) {
  if(!resource) return NULL_HANDLE_ERROR;

  resource->handle = malloc(resource->size * sizeof(char));
  if(!resource->handle) return ALLOCATION_ERROR;

  memcpy(resource->handle, data, resource->size * sizeof(char));

  return SUCCESS;
}

void resource_destroy(const Resource *resource) {
  if(!resource) return;

  free(resource->handle);
}

void resources_destroy() {
  for(unsigned index = 0; index < resources_length; ++index)
    resource_destroy(resources + index);
}
