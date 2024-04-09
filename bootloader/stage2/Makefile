# Makefile for stage 2

# Compiler, assembler, and linker definitions
ASM=nasm
CC=gcc
LD=ld
OBJCOPY=objcopy

# Paths
INCLUDE_DIR=../../include

# Compiler flags
CFLAGS=-m32 -ffreestanding -nostartfiles -nodefaultlibs -I$(INCLUDE_DIR) -Og -MMD -MP

# Source files
ASM_SRC=stage2.asm
C_SRC=../../common/ata/ata.c \
	../../common/lib/io.c \
	../../common/lib/vga_text.c \
	../../common/lib/string_memory.c \
	../../fs/FAT/fat.c \

# Object files
ASM_OBJ=$(ASM_SRC:.asm=.o)
C_OBJ=$(C_SRC:.c=.o)

# Dependency files
DEP=$(C_OBJ:.o=.d)

# Output binary
OUT_BIN=stage2.bin
INTERMEDIATE_ELF=stage2.elf

# Linker script
LINKER_SCRIPT=bootloader.ld

.PHONY: all clean

# Default target
all: clean $(OUT_BIN)

# Assemble assembly source code
%.o: %.asm
	$(ASM) -f elf32 $< -o $@

# Compile C source code
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into an ELF binary, then convert to flat binary, then add header and pad
$(OUT_BIN): $(ASM_OBJ) $(C_OBJ)
	$(LD) -m elf_i386 -T $(LINKER_SCRIPT) $(ASM_OBJ) $(C_OBJ) -o $(INTERMEDIATE_ELF)
	$(OBJCOPY) -O binary $(INTERMEDIATE_ELF) $(OUT_BIN)
	
	# Step 1: Prepend a 4-byte placeholder for the size
	dd if=/dev/zero bs=4 count=1 | cat - $(OUT_BIN) > temp.bin && mv temp.bin $(OUT_BIN)

	# Step 2: Pad the file size to a multiple of 512 bytes, excluding the initial 4-byte placeholder
	@{ \
		SIZE=$$(stat -c%s $(OUT_BIN)); \
		PAD_SIZE=$$(( (512 - (SIZE % 512)) % 512)); \
		dd if=/dev/zero bs=1 count=$$PAD_SIZE oflag=append conv=notrunc of=$(OUT_BIN); \
	}

	# Step 3: Calculate the final size and overwrite the placeholder with the actual size
	@{ \
		FINAL_SIZE=$$(stat -c%s $(OUT_BIN)); \
		printf '%08x' $$((FINAL_SIZE / 512)) | sed 's/\(..\)/\1 /g' | tac | xxd -r -p > size.bin; \
		dd if=size.bin of=$(OUT_BIN) bs=1 count=4 conv=notrunc; \
		rm -f size.bin; \
	}

# Clean up
clean:
	rm -f $(ASM_OBJ) $(C_OBJ) $(OUT_BIN) $(INTERMEDIATE_ELF) $(DEP)

# Include the dependency files
-include $(DEP)