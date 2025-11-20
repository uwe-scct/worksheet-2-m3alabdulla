# Top-level Makefile for Worksheet 2 - minimal solution
NASM = nasm
CC = gcc
LD = ld
CFLAGS = -m32 -ffreestanding -O0 -g
LDFLAGS = -m elf_i386

SRC = source
ISO = iso
OUT = build

KERNEL_ELF = $(OUT)/kernel.elf
LOADER_OBJ = $(OUT)/loader.o
KERNEL_OBJ = $(OUT)/kernel.o

.PHONY: all clean iso run

all: $(KERNEL_ELF)

$(OUT):
	mkdir -p $(OUT)

$(LOADER_OBJ): $(SRC)/loader.asm | $(OUT)
	$(NASM) -f elf $(SRC)/loader.asm -o $(LOADER_OBJ)

$(KERNEL_OBJ): $(SRC)/kernel.c | $(OUT)
	$(CC) $(CFLAGS) -c $(SRC)/kernel.c -o $(KERNEL_OBJ)

$(KERNEL_ELF): $(LOADER_OBJ) $(KERNEL_OBJ) $(SRC)/link.ld
	$(LD) $(LDFLAGS) -T $(SRC)/link.ld -o $(KERNEL_ELF) $(LOADER_OBJ) $(KERNEL_OBJ)

iso: all
	@echo "Creating ISO image (requires grub-mkrescue)..."
	mkdir -p $(ISO)/boot
	mkdir -p $(ISO)/boot/grub
	cp $(KERNEL_ELF) $(ISO)/boot/kernel.elf
	# copy grub config (already present)
	grub-mkrescue -o os.iso $(ISO) >/dev/null 2>&1 || \
		(echo "grub-mkrescue failed — install grub (grub-pc-bin) and xorriso/genisoimage"; exit 1)
	@echo "Created os.iso"

run: iso
	@echo "Running QEMU..."
	qemu-system-i386 -cdrom os.iso -m 32M -serial stdio

clean:
	rm -rf $(OUT) os.iso
