#include "util/error.h"
#include "util/file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

File output;
File input;
File resource;

#define full_model                                                                                                                              \
  "#include \"util/error.h\"\n"                                                                                                                 \
  "#include \"util/resource.h\"\n"                                                                                                              \
  "\n"                                                                                                                                          \
  "struct ResourceBundle {\n"                                                                                                                   \
  "  const char *name;\n"                                                                                                                       \
  "  const char *data;\n"                                                                                                                       \
  "  ResourceType type;\n"                                                                                                                      \
  "  unsigned size;\n"                                                                                                                          \
  "};\n"                                                                                                                                        \
  "\n"                                                                                                                                          \
  "const unsigned resource_count = %u;\n"                                                                                                       \
  "\n"                                                                                                                                          \
  "Error resource_load(Resource *resources) {\n"                                                                                                \
  "  struct ResourceBundle bundles[] = {\n"                                                                                                     \
  "%s"                                                                                                                                          \
  "  };\n"                                                                                                                                      \
  "\n"                                                                                                                                          \
  "  for(unsigned index = 0; index < resource_count; ++index) {\n"                                                                              \
  "    if(resource_create(resources + index, bundles[index].type, bundles[index].name, bundles[index].data, bundles[index].size) != SUCCESS)\n" \
  "      return RESOURCE_LOAD_ERROR;\n"                                                                                                         \
  "  }\n"                                                                                                                                       \
  "\n"                                                                                                                                          \
  "  return SUCCESS;\n"                                                                                                                         \
  "}\n"

#define resource_model             \
  "    (struct ResourceBundle){\n" \
  "      .name = \"%.*s\",\n"      \
  "      .data = (char[]){ %s},\n" \
  "      .type = %s,\n"            \
  "      .size = %u,\n"            \
  "    },\n"

char *dbuffer; // Data Buffer
char *rbuffer; // Resource Buffer
char *buffer;  // Full Buffer

unsigned rbuffer_length;

char *path;

unsigned resource_count;

void clear();
int parse_resource(const char **fields, unsigned *lengths);
const char *find_type(const char *type, unsigned length);

int main(int argc, char **argv) {
  if(argc < 3) return 1;

  atexit(clear);

  // Create files
  if(file_create(&output) != SUCCESS) {
    printf("ERROR: Failed to create output file\n");
    return 1;
  }

  if(file_create(&input) != SUCCESS) {
    printf("ERROR: Failed to create input file\n");
    return 1;
  }

  if(file_create(&resource) != SUCCESS) {
    printf("ERROR: Failed to create resource file\n");
    return 1;
  }

  if(file_open(output, argv[1], &(FilePermission){ .write = 1, .create = 1, .truncate = 1 }) != SUCCESS) {
    printf("ERROR: Failed to open output file '%s'\n", argv[1]);
    return 1;
  }

  if(file_open(input, argv[2], &(FilePermission){ .read = 1 }) != SUCCESS) {
    printf("ERROR: Failed to open input file '%s'\n", argv[2]);
    return 1;
  }

  const char *data = file_get_buffer(input);
  unsigned size = file_get_size(input);

  // Read CSV
  const char *fields[3];
  unsigned lengths[3];
  unsigned char field = 0;
  unsigned char is_field = 0;

  for(unsigned index = 0; index < size; ++index) {
    if(data[index] == '"' && (index == 0 || data[index - 1] != '\\')) {
      is_field = !is_field;

      if(!is_field) {
        lengths[field] = data + index - fields[field];
        field++;
        continue;
      }

      if(field == 3) {
        printf("ERROR: Extra field\n");
        continue;
      }

      fields[field] = data + index + 1;
      continue;
    }

    if(!is_field && data[index] == '\n') {
      if(field < 3) {
        printf("ERROR: Missing fields\n");
        continue;
      }

      if(parse_resource(fields, lengths))
        printf("ERROR: Failed to parse resource\n");

      field = 0;
    }
  }

  if(is_field) {
    printf("ERROR: Uncomplete field\n");
    return 1;
  }

  unsigned length = snprintf(NULL, 0, full_model, resource_count, rbuffer);

  buffer = malloc(length + 1);
  if(!buffer) {
    printf("ERROR: Failed to allocate full buffer\n");
    return 1;
  }

  snprintf(buffer, length + 1, full_model, resource_count, rbuffer);

  if(file_write(output, 0, buffer, length) != SUCCESS) {
    printf("ERROR: Failed to write to output file\n");
    return 1;
  }

  if(file_flush(output) != SUCCESS) {
    printf("ERROR: Failed to flush output file\n");
    return 1;
  }
}

int parse_resource(const char **fields, unsigned *lengths) {
  static const char hex[] = "0123456789abcdef";

  // Setup fields as pure strings(they are substrings)
  void *tmp = realloc(path, lengths[0] + 1);
  if(!tmp) {
    printf("ERROR: Failed to allocate resource path\n");
    return 1;
  }
  path = tmp;
  path[lengths[0]] = 0;

  memcpy(path, fields[0], lengths[0]);
  // memcpy(name, fields[1], lengths[1]);
  // memcpy(type, fields[2], lengths[2]);

  if(file_open(resource, path, &(FilePermission){ .read = 1 }) != SUCCESS) {
    printf("ERROR: Failed to open resource file '%s'\n", path);
    return 1;
  }

  const char *data = file_get_buffer(resource);
  unsigned size = file_get_size(resource);

  // Setup data buffer
  // "0x.., " -> 6 bytes
  tmp = realloc(dbuffer, 6 * size + 1);
  if(!tmp) {
    printf("ERROR: Failed to allocate data buffer\n");
    return 1;
  }
  dbuffer = tmp;
  dbuffer[6 * size] = 0;

  for(unsigned byte = 0, offset = 0; byte < size; ++byte, offset += 6) {
    dbuffer[offset + 0] = '0';
    dbuffer[offset + 1] = 'x';
    dbuffer[offset + 2] = hex[(data[byte] >> 4) & 0xf];
    dbuffer[offset + 3] = hex[data[byte] & 0xf];
    dbuffer[offset + 4] = ',';
    dbuffer[offset + 5] = ' ';
  }

  const char *type = find_type(fields[2], lengths[2]);
  unsigned length = snprintf(NULL, 0, resource_model, lengths[1], fields[1], dbuffer, type, size);

  tmp = realloc(rbuffer, rbuffer_length + length + 1);
  if(!tmp) {
    printf("ERROR: Failed to allocate resource buffer\n");
    return 1;
  }
  rbuffer = tmp;

  snprintf(rbuffer + rbuffer_length, length + 1, resource_model, lengths[1], fields[1], dbuffer, type, size);

  rbuffer_length += length;

  ++resource_count;

  return 0;
}

void clear() {
  free(dbuffer);
  free(rbuffer);
  free(buffer);

  free(path);

  // if(file_close(resource) != SUCCESS)
  //   printf("ERROR: Failed to close resource file\n");

  if(file_close(input) != SUCCESS)
    printf("ERROR: Failed to close input file\n");

  if(file_close(output) != SUCCESS)
    printf("ERROR: Failed to close output file\n");

  file_destroy(resource);
  file_destroy(input);
  file_destroy(output);
}

const char *find_type(const char *type, unsigned length) {
  if(!strncmp(type, "spirv", length))
    return "TYPE_SPIRV";
  if(!strncmp(type, "unknown", length))
    return "TYPE_UNKNOWN";

  printf("ERROR: Invalid resource type '%.*s'\n", length, type);
  return "TYPE_UNKNOWN";
}
