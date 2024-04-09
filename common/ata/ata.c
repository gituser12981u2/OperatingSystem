#include "lib/io.h"
#include "lib/vga_text.h"

#define ATA_PRIMARY_DATA_PORT         0x1F0
#define ATA_PRIMARY_ERROR_PORT        0x1F1 // Same as Features Port
#define ATA_PRIMARY_SECTOR_COUNT_PORT 0x1F2
#define ATA_PRIMARY_LBA_LOW_PORT      0x1F3
#define ATA_PRIMARY_LBA_MID_PORT      0x1F4
#define ATA_PRIMARY_LBA_HIGH_PORT     0x1F5
#define ATA_PRIMARY_DRIVE_PORT        0x1F6
#define ATA_PRIMARY_STATUS_PORT       0x1F7 // Same as Command Port
#define ATA_PRIMARY_COMMAND_PORT      0x1F7
#define ATA_IDENTIFY_COMMAND          0xEC

void detect_ata_device() {
    // Select master drive on primary channel with LBA mode (bit 6 set)
    outb(ATA_PRIMARY_DRIVE_PORT, 0xA0);
    
    // Send IDENTIFY command
    outb(ATA_PRIMARY_STATUS_PORT, ATA_IDENTIFY_COMMAND);
    
    // Read status to see if device is present
    uint8_t status = inb(ATA_PRIMARY_STATUS_PORT);
    if (status) {
        print_to_screen("ATA DEVICE FOUND!!.\n");
        while (!(inb(ATA_PRIMARY_STATUS_PORT) & (1 << 3)));

        // Read identification data
        uint16_t data[256];
        for (int i = 0; i < 256; i++) {
            data[i] = inw(ATA_PRIMARY_DATA_PORT);
        }

        if (data[49] & (1 << 9)) {
            print_to_screen("LBA supported\n");
        } else {
            print_to_screen("LBA not supported\n");
        }
        uint32_t total_sectors = (uint32_t)data[61] << 16 | data[60];
        if (data[63] & (1 << 0)) {
            print_to_screen("DMA mode 0 supported\n");
        }

        if (data[64] & (1 << 0)) {
            print_to_screen("PIO mode 0 supported\n");
        }

    } else {
        // No device present
        print_to_screen("No ATA device found.\n");
    }
}

void read_sector_lba(uint32_t lba, uint8_t sector_count, void* buffer) {
    // Select the drive and LBA mode
    outb(ATA_PRIMARY_DATA_PORT, 0xE0 | ((lba >> 24) & 0x0F));

    // Set the sector count
    outb(ATA_PRIMARY_SECTOR_COUNT_PORT, sector_count);

    // Set the LBA address (low, mid, high)
    outb(ATA_PRIMARY_LBA_LOW_PORT, lba & 0xFF);
    outb(ATA_PRIMARY_LBA_MID_PORT, (lba >> 8) & 0xFF);
    outb(ATA_PRIMARY_LBA_HIGH_PORT, (lba >> 16) & 0xFF);

    // Send the READ SECTORS command
    outb(ATA_PRIMARY_COMMAND_PORT, 0x20); // 0x20 = READ SECTORS

    // Wait for the drive to be ready to transfer data
    while (!(inb(ATA_PRIMARY_STATUS_PORT) & (1 << 3)));

    // Read the data from the data port
    for (int i = 0; i < sector_count * 256; i++) {
        ((uint16_t*)buffer)[i] = inw(ATA_PRIMARY_DATA_PORT);
    }
}