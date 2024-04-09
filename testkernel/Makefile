# Makefile for testkernel

# Define assembler and flags
ASM=nasm
ASMFLAGS=-f elf64 # For ELF object file
LDFLAGS=-nostdlib -m elf_x86_64 -Tkernel.ld # Linker flags
OBJCOPY=objcopy # For converting ELF to binary format

# Define source and target files
KERNEL_ASM_SRC=entry.asm
KERNEL_OBJ=entry.o
KERNEL_BIN=kernel.bin

.PHONY: all clean 

# Default target that builds the kernel binary
all: $(KERNEL_BIN)

$(KERNEL_BIN): $(KERNEL_OBJ) kernel.ld
	ld $(LDFLAGS) -o $(KERNEL_BIN).elf $(KERNEL_OBJ)
	objcopy -O binary $(KERNEL_BIN).elf $(KERNEL_BIN)


$(KERNEL_OBJ): $(KERNEL_ASM_SRC)
	$(ASM) $(ASMFLAGS) $(KERNEL_ASM_SRC) -o $(KERNEL_OBJ)

# Clean up the binary and object files
clean:
	rm -f $(KERNEL_BIN) $(KERNEL_OBJ) $(KERNEL_BIN).elf