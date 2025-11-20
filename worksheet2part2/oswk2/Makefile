# Makefile for oswk2 (Worksheet 2)
AS = nasm
CC = gcc
LD = ld
CFLAGS = -m32 -ffreestanding -O0 -g -Wall
ASFLAGS = -f elf -d ELF_TYPE

SRCDIR = source
DRIVERS = drivers
BUILDDIR = build

OBJS = $(BUILDDIR)/loader.o \
       $(BUILDDIR)/kernel.o \
       $(BUILDDIR)/pic.o \
       $(BUILDDIR)/keyboard.o \
       $(BUILDDIR)/interrupts.o \
       $(BUILDDIR)/load_idt.o \
       $(BUILDDIR)/interrupt_handlers.o

.PHONY: all build iso run clean

all: build

# top-level build target produces kernel.elf in build/
build: $(BUILDDIR)/kernel.elf
	@echo "Build complete: $(BUILDDIR)/kernel.elf"

# assemble loader.asm
$(BUILDDIR)/loader.o: $(SRCDIR)/loader.asm
	@mkdir -p $(BUILDDIR)
	$(AS) $(ASFLAGS) $< -o $@

# compile kernel.c
$(BUILDDIR)/kernel.o: $(SRCDIR)/kernel.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# compile drivers
$(BUILDDIR)/pic.o: $(DRIVERS)/pic.c $(DRIVERS)/pic.h $(DRIVERS)/io.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/keyboard.o: $(DRIVERS)/keyboard.c $(DRIVERS)/keyboard.h $(DRIVERS)/io.h $(DRIVERS)/pic.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/interrupts.o: $(DRIVERS)/interrupts.c $(DRIVERS)/interrupts.h
	$(CC) $(CFLAGS) -c $< -o $@

# assemble load_idt.asm (implements load_idt)
$(BUILDDIR)/load_idt.o: $(DRIVERS)/load_idt.asm
	$(AS) $(ASFLAGS) $< -o $@

# assemble interrupt handlers asm
$(BUILDDIR)/interrupt_handlers.o: $(DRIVERS)/interrupt_handlers.asm
	$(AS) $(ASFLAGS) $< -o $@

# link kernel.elf
$(BUILDDIR)/kernel.elf: $(OBJS)
	$(LD) -m elf_i386 -T $(SRCDIR)/link.ld -o $@ $(OBJS)

# create ISO using GRUB (requires grub-mkrescue/xorriso)
iso: $(BUILDDIR)/kernel.elf
	@mkdir -p iso/boot/grub
	@cp $(BUILDDIR)/kernel.elf iso/boot/kernel.elf
	@cp $(SRCDIR)/menu.lst iso/boot/grub/menu.lst 2>/dev/null || true
	@echo "Creating os.iso (requires grub-mkrescue/xorriso)..."
	@grub-mkrescue -o os.iso iso >/dev/null 2>&1 || (echo "grub-mkrescue failed — install grub-pc-bin and xorriso" ; exit 1)
	@echo "Created os.iso"

# convenience: run QEMU via run_qemu.sh
run: iso
	./run_qemu.sh

clean:
	-rm -rf $(BUILDDIR) os.iso iso

# End of Makefile

# build multiboot header
$(BUILDDIR)/multiboot_header.o: $(SRCDIR)/multiboot_header.asm
	@mkdir -p $(BUILDDIR)
	$(AS) $(ASFLAGS) $< -o $@
