# Makefile for entire bootloader

# Define paths
STAGE1_DIR=./stage1
STAGE2_DIR=./stage2
IMG=bootloader.img

# Define qemu version
QEMU=qemu-system-x86_64

.PHONY: all clean run stage1 stage2

# Default target that builds everything and creates the disk image
all: clean $(IMG)

# Default target
stage1:
	$(MAKE) -C $(STAGE1_DIR)

# Build and assemble Stage 1
stage2:
	$(MAKE) -C $(STAGE2_DIR)

# Create disk image and combine Stage 1 and Stage 2
$(IMG): stage1 stage2
	dd if=/dev/zero of=$(IMG) bs=1M count=10
	dd if=$(STAGE1_DIR)/bootloader.bin of=$(IMG) bs=512 conv=notrunc
	dd if=$(STAGE2_DIR)/stage2.bin of=$(IMG) bs=512 seek=1 conv=notrunc

# Run the combined bootloader in QEMU (hdd)
run: $(IMG)
	$(QEMU) -drive file=$(IMG),format=raw,index=0,media=disk,if=ide

# Run with GDB
gdb: $(IMG)
	$(QEMU) -drive file=$(IMG),format=raw,index=0,media=disk,if=ide

# Run the combined bootloader in QEMU (floppy)
floppy: $(IMG)
	$(QEMU) -drive file=$(IMG),format=raw,index=0,if=floppy

# Clean up the binary files
clean:
	$(MAKE) -C $(STAGE1_DIR) clean
	$(MAKE) -C $(STAGE2_DIR) clean
	rm -f $(IMG)