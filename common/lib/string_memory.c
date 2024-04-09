#include "lib/string_memory.h"
#include "lib/io.h"

int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++, str2++;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

// Comparison function for FAT 8.3 filenames
int fat_strcmp(const char *fatFilename, const char *filename) {

    char fatName[12]; // 11 characters + null-terminator
    int i, j;

    // Copy and space-pad the input filename into fatName
    for (i = 0, j = 0; filename[i] != '\0' && i < 11; i++, j++) {
        if (filename[i] == '.' && j < 8) { // Skip the dot in the input filename
            while (j < 8) { // Space-pad the rest of the name part
                fatName[j++] = ' ';
            }
        } else {
            fatName[j] = filename[i]; // Copy filename characters
        }
    }
    while (j < 11) { // Space-pad the rest if filename is shorter than 11 characters
        fatName[j++] = ' ';
    }
    fatName[j] = '\0'; // Null-terminate the FAT filename

    // Now compare the space-padded fatName with the FAT directory entry name
    for (i = 0; i < 11; i++) {
        if (fatFilename[i] != fatName[i]) {
            return fatFilename[i] - fatName[i];
        }
    }
    return 0; // The names match
}

void to_upper(char *str) {
    while (*str) {
        if (*str >= 'a' && *str <= 'z') {
            *str -= 32; // ASCII difference between lowercase and uppercase
        }
        str++;
    }
}

void strncpy(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
}

void *memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}