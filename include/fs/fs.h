#ifndef FS_H
#define FS_H

#include "../lib/io.h"

// Initialize the filesystem
void fs_init();

// Read a file into a buffer
int fs_read_file(const char* path, uint8_t* buffer, size_t buffer_size);

// List the contents of a directory
int fs_list_directory(const char* path);

#endif // FS_H