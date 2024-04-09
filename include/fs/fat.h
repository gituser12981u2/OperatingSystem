#ifndef FAT_H
#define FAT_H

#include "../lib/io.h"

typedef struct {
    char filename[11]; // 8+3 format, space-padded
    uint8_t attr;
    uint8_t reserved;
    uint8_t createTimeTenth;
    uint16_t createTime;
    uint16_t createDate;
    uint16_t accessDate;
    uint16_t firstClusterHigh;
    uint16_t writeTime;
    uint16_t writeDate;
    uint16_t firstClusterLow;
    uint32_t size;
} __attribute__((packed)) DirectoryEntry;

// FAT attributes
#define ATTR_READ_ONLY  0x01
#define ATTR_HIDDEN     0x02
#define ATTR_SYSTEM     0x04
#define ATTR_VOLUME_ID  0x08
#define ATTR_DIRECTORY  0x10
#define ATTR_ARCHIVE    0x20

// Function prototypes
void read_sector_lba(uint32_t lba, uint8_t sector_count, void* buffer);

#endif // FAT_H