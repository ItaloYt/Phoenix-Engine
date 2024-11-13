#ifndef PHOENIX_UTIL_FILE_H
#define PHOENIX_UTIL_FILE_H

#include "util/error.h"
typedef struct File *File;

typedef struct {
  unsigned char read : 1;
  unsigned char write : 1;
  unsigned char create : 1;
  unsigned char truncate : 1;
} FilePermission;

Error file_create(File *file);
Error file_open(File file, const char *path, const FilePermission *perm);
Error file_write(File file, unsigned offset, const void *buffer, unsigned size);
Error file_flush(File file);
Error file_close(File file);
void file_destroy(File file);

int file_get_handle(File file);
const FilePermission *file_get_perm(File file);
const char *file_get_path(File file);
unsigned file_get_path_length(File file);
const char *file_get_buffer(File file);
unsigned file_get_size(File file);

#endif
