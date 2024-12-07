#include "util/file.h"
#include "util/error.h"
#include <fcntl.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct FileNative {
  int handle;
};

Error file_open(File *file, const FilePermission *perm, const char *path) {
  if(!file || !perm) return NULL_HANDLE_ERROR;

  if(!file->native) {
    file->native = malloc(sizeof(struct FileNative));
    if(!file->native) return ALLOCATION_ERROR;
  }

  int flags = (perm->read && perm->write ? O_RDWR : (perm->read ? O_RDONLY : (perm->write ? O_WRONLY : 0)));
  flags |= (perm->create ? O_CREAT : 0) | (perm->truncate ? O_TRUNC : 0);

  int mode = (perm->create ? S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH : 0);

  file->native->handle = open(path, flags, mode);
  if(file->native->handle == -1) return FILE_OPEN_ERROR;

  // Create buffer
  file->size = lseek(file->native->handle, 0, SEEK_END);
  if(file->size == -1) return FILE_SEEK_ERROR;
  if(lseek(file->native->handle, 0, SEEK_SET) == -1) return FILE_SEEK_ERROR;

  if(file->size == 0) return SUCCESS;

  void *tmp = realloc(file->buffer, file->size * sizeof(char));
  if(!tmp) return ALLOCATION_ERROR;
  file->buffer = tmp;

  if(!perm->read) return SUCCESS;
  if(read(file->native->handle, file->buffer, file->size) == -1) return FILE_READ_ERROR;

  return SUCCESS;
}

Error file_write(File *file, unsigned offset, const void *buffer, unsigned size) {
  if(!file || !buffer) return NULL_HANDLE_ERROR;

  void *tmp = NULL;

  // Resize buffer
  if(size + offset > file->size) {
    file->size = size + offset;
    tmp = realloc(file->buffer, file->size * sizeof(char));
    if(!tmp) return ALLOCATION_ERROR;
    file->buffer = tmp;
  }

  memcpy(file->buffer + offset, buffer, size);

  return SUCCESS;
}

Error file_flush(File *file) {
  if(!file) return NULL_HANDLE_ERROR;

  if(ftruncate(file->native->handle, file->size) == -1) return FILE_TRUNCATE_ERROR;
  if(write(file->native->handle, file->buffer, file->size) == -1) return FILE_WRITE_ERROR;

  return SUCCESS;
}

Error file_close(File *file) {
  if(!file) return NULL_HANDLE_ERROR;

  if(close(file->native->handle) == -1) return FILE_CLOSE_ERROR;

  return SUCCESS;
}

void file_destroy(File *file) {
  if(!file) return;

  free(file->native);
  file->native = 0x0;

  free(file->buffer);
  file->buffer = 0x0;
}
