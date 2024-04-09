# Makefile for stage 1

# Define assembler and qemu version
ASM=nasm
QEMU=qemu-system-x86_64

# Source and binary
SRC=bootloader.asm
BIN=bootloader.bin

# Default target
all: clean $(BIN)

# Additional target for building and running
build-run: all run

# Assemble the bootloader
$(BIN): $(SRC)
	$(ASM) -f bin -o $(BIN) $(SRC)

# Run the bootloader in QEMU
run: $(BIN)
	$(QEMU) -drive file=$(BIN),format=raw,index=0,if=floppy

# Clean up the binary files
clean:
	rm -f $(BIN)