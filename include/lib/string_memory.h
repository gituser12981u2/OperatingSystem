#ifndef STRING_MEMORY_H
#define STRING_MEMORY_H

#include "../lib/io.h"

int strcmp(const char *str1, const char *str2);
int fat_strcmp(const char *fatFilename, const char *filename);
void to_upper(char *str);
void strncpy(char *dest, const char *src, size_t n);
void *memcpy(void *dest, const void *src, size_t n);

#endif // STRING_MEMORY_H
