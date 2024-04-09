# Root Makefile for OS

.PHONY: all bootloader kernel run clean

BOOTLOADER_DIR=./bootloader
KERNEL_DIR=./testkernel
OS_IMG=./os.img
QEMU=qemu-system-x86_64

all: clean bootloader kernel disk_image run

bootloader:
	$(MAKE) -C $(BOOTLOADER_DIR)

kernel:
	$(MAKE) -C $(KERNEL_DIR)

# Assemble the final disk image
disk_image: bootloader
	dd if=/dev/zero of=$(OS_IMG) bs=1M count=64
	dd if=$(BOOTLOADER_DIR)/bootloader.img of=$(OS_IMG) bs=1 count=1024 conv=notrunc
	mkfs.fat -F 32 -S 512 --offset 2 $(OS_IMG)

# Create temporary mount point
	mkdir -p ~/os_temp

# Mount the FAT-formatted disk image
	sudo mount -o loop,offset=1024 $(OS_IMG) ~/os_temp

# Copy the kernel and any other OS files to the mounted image
	sudo cp $(KERNEL_DIR)/kernel.bin ~/os_temp/
	sync
	sleep 1
	sudo umount ~/os_temp || sudo umount -l ~/os_temp
	if mountpoint -q ~/os_temp; then echo "Mount point busy, manual cleanup may be required"; else rmdir ~/os_temp; fi
	
# Run the OS in QEMU
run: $(OS_IMG)
	$(QEMU) -drive file=$(OS_IMG),format=raw,index=0,media=disk,if=ide

clean:
	rm -f $(OS_IMG)
	$(MAKE) -C $(BOOTLOADER_DIR) clean
	$(MAKE) -C $(KERNEL_DIR) clean

