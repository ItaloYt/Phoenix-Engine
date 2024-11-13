#include "util/file.h"
#include "util/error.h"
#include <fcntl.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct File {
  int handle;
  FilePermission perm;
  char path[PATH_MAX];
  unsigned path_length;
  char *buffer;
  unsigned size;
};

Error file_create(File *file) {
  if(!file) return NULL_HANDLE_ERROR;

  *file = malloc(sizeof(struct File));
  if(!*file) return ALLOCATION_ERROR;

  (*file)->handle = -1;
  (*file)->path_length = 0;
  (*file)->size = 0;
  (*file)->buffer = malloc(0);
  if(!(*file)->buffer) return ALLOCATION_ERROR;

  return SUCCESS;
}

Error file_open(File file, const char *path, const FilePermission *perm) {
  if(!file || !path) return NULL_HANDLE_ERROR;

  file->path_length = strlen(path);
  (void)memcpy(file->path, path, file->path_length * sizeof(char));

  file->perm = *perm;

  int flags = (perm->read && perm->write ? O_RDWR : (perm->read ? O_RDONLY : (perm->write ? O_WRONLY : 0)));
  flags |= (perm->create ? O_CREAT : 0) | (perm->truncate ? O_TRUNC : 0);

  int mode = (perm->create ? S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH : 0);

  file->handle = open(path, flags, mode);
  if(file->handle == -1) return FILE_OPEN_ERROR;

  // Create buffer
  file->size = lseek(file->handle, 0, SEEK_END);
  if(file->size == -1) return FILE_SEEK_ERROR;
  if(lseek(file->handle, 0, SEEK_SET) == -1) return FILE_SEEK_ERROR;

  if(file->size == 0) return SUCCESS;

  void *tmp = realloc(file->buffer, file->size * sizeof(char));
  if(!tmp) return ALLOCATION_ERROR;
  file->buffer = tmp;

  if(!perm->read) return SUCCESS;
  if(read(file->handle, file->buffer, file->size) == -1) return FILE_READ_ERROR;

  return SUCCESS;
}

Error file_write(File file, unsigned offset, const void *buffer, unsigned size) {
  if(!file || !buffer) return NULL_HANDLE_ERROR;
  if(!file->perm.write) return FILE_INVALID_PERMISSION_ERROR;

  void *tmp = NULL;

  // Resize buffer
  if(size + offset > file->size) {
    file->size = size + offset;
    tmp = realloc(file->buffer, file->size * sizeof(char));
    if(!tmp) return ALLOCATION_ERROR;
    file->buffer = tmp;
  }

  (void)memcpy(file->buffer + offset, buffer, size);

  return SUCCESS;
}

Error file_flush(File file) {
  if(!file) return NULL_HANDLE_ERROR;
  if(!file->perm.write) return FILE_INVALID_PERMISSION_ERROR;

  if(ftruncate(file->handle, file->size) == -1) return FILE_TRUNCATE_ERROR;
  if(write(file->handle, file->buffer, file->size) == -1) return FILE_WRITE_ERROR;

  return SUCCESS;
}

Error file_close(File file) {
  if(!file) return NULL_HANDLE_ERROR;
  if(file->handle == -1) return SUCCESS;

  if(close(file->handle) == -1) return FILE_CLOSE_ERROR;

  file->handle = -1;

  return SUCCESS;
}

void file_destroy(File file) {
  if(!file) return;

  free(file->buffer);
  free(file);
}

int file_get_handle(File file) { return (!file ? -1 : file->handle); }
const FilePermission *file_get_perm(File file) { return (!file ? NULL : &file->perm); }
const char *file_get_path(File file) { return (!file ? NULL : file->path); }
unsigned file_get_path_length(File file) { return (!file ? 0 : file->path_length); }
const char *file_get_buffer(File file) { return (!file ? NULL : file->buffer); }
unsigned file_get_size(File file) { return (!file ? 0 : file->size); }
