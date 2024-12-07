#ifndef PHOENIX_UTIL_FILE_H
#define PHOENIX_UTIL_FILE_H

#include "util/error.h"

typedef struct FileNative FileNative;

typedef struct {
  unsigned char read : 1;
  unsigned char write : 1;
  unsigned char create : 1;
  unsigned char truncate : 1;
} FilePermission;

typedef struct File {
  FileNative *native;
  char *buffer;
  unsigned size;
} File;

Error file_open(File *file, const FilePermission *perm, const char *path);
Error file_write(File *file, unsigned offset, const void *buffer, unsigned size);
Error file_flush(File *file);
Error file_close(File *file);
void file_destroy(File *file);

#endif
