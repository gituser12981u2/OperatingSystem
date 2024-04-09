#include "fs/fat.h"
#include "lib/io.h"
#include "lib/string_memory.h"

extern void read_sector_lba(uint32_t lba, uint8_t sector_count, void* buffer);

// Globals storing FAT parameters
uint32_t FatTableSectorStart;
uint32_t ClusterHeapSectorStart;
uint32_t RootDirFirstCluster;
uint32_t SectorsPerCluster;

uint32_t read_next_cluster(uint32_t currentCluster) {
    uint32_t fatSector = FatTableSectorStart + (currentCluster / (512 / 4));
    uint32_t fatOffset = currentCluster % (512 / 4);
    uint32_t nextCluster;
    uint8_t sectorBuffer[512];

    read_sector_lba(fatSector, 1, sectorBuffer);
    nextCluster = *((uint32_t*)&sectorBuffer[fatOffset]) & 0x0FFFFFFF; // Masking to get 28 bits

    return nextCluster;
}

int find_file_in_root(const char* filename, DirectoryEntry* entry) {
    char upperFilename[12]; // Buffer for uppercase filename
    strncpy(upperFilename, filename, sizeof(upperFilename) - 1); // Copy and protect from overflow
    upperFilename[sizeof(upperFilename) - 1] = '\0';
    to_upper(upperFilename); // Convert the filename to uppercase

    uint32_t currentCluster = RootDirFirstCluster;
    uint8_t sectorBuffer[512];
    DirectoryEntry* dirEntries = (DirectoryEntry*)sectorBuffer;
    int entriesPerSector = 512 / sizeof(DirectoryEntry);

    // Loop through the cluster chain for the root directory
    while (currentCluster < 0x0FFFFFF8) {
        uint32_t sector = ClusterHeapSectorStart + (currentCluster - 2) * SectorsPerCluster;

        // Read sectors in the current cluster
        for (int i = 0; i < SectorsPerCluster; ++i) {
            read_sector_lba(sector + i, 1, sectorBuffer);

            // Check for each directory entry in the sector
            for (int j = 0; j < entriesPerSector; ++j) {
                if (fat_strcmp((char*)dirEntries[j].filename, filename) == 0) {
                    // Found the file now copy its directory entry
                    memcpy(entry, &dirEntries[j], sizeof(DirectoryEntry));
                    return 1; // Success
                }
            }
        }
        // Move to the next cluster in the chain
        currentCluster = read_next_cluster(currentCluster);
    }
    return 0; // File not found
}
