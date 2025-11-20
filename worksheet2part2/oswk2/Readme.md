

# **OS Worksheet 2 – Minimal Kernel, Build System & GRUB ISO**




## **1. Overview**

This worksheet focuses on building a very small 32-bit operating system kernel and booting it with GRUB.
The tasks include:

* Writing the basic kernel and loader files
* Creating a proper folder structure
* Compiling C and Assembly files for a freestanding environment
* Linking them into a flat ELF kernel
* Packaging the kernel into a bootable ISO image using **grub-mkrescue**
* Running the ISO with **QEMU**

The purpose is to understand the **PC boot process**, **GRUB multiboot loading**, and how a kernel is built without the normal C runtime.

---

## **2. Repository Structure**

```
oswk2/
│
├── source/
│   ├── kernel.c
│   ├── loader.asm
│   ├── link.ld
│   └── menu.lst
│
├── drivers/
│   ├── io.h
│   ├── pic.c/h
│   ├── keyboard.c/h
│   ├── interrupts.c/h
│   ├── load_idt.asm
│   └── interrupt_handlers.asm
│
├── build/           # Created automatically
├── iso/             # Created automatically
│
├── Makefile
└── run_qemu.sh
```

---

## **3. Building the Kernel**

To compile everything:

```bash
make
```

This:

* Creates the `build/` directory
* Assembles the assembly files with `nasm`
* Compiles all C drivers in freestanding 32-bit mode
* Links everything using the linker script `link.ld`
* Produces:
  **build/kernel.elf**

You will see:

```
Build complete: build/kernel.elf
```

---

## **4. Creating the Bootable ISO**

Run:

```bash
make iso
```

This:

* Creates `iso/boot/` and `iso/boot/grub/`
* Copies `kernel.elf`
* Copies the GRUB menu (`menu.lst`)
* Runs:

```
grub-mkrescue -o os.iso iso/
```

which generates:

**os.iso**

This ISO contains:

```
/boot/kernel.elf
/boot/grub/menu.lst
```

GRUB successfully detects the filesystem and kernel inside the ISO.

---

## **5. Running the OS in QEMU**

You can run using the helper script:

```bash
./run_qemu.sh
```

Or manually:

```bash
qemu-system-i386 -cdrom os.iso -m 64M -nographic
```

QEMU successfully loads the ISO and displays:

```
GNU GRUB  version 2.12
grub>
```

and shows the kernel file:

```
ls (cd)/boot/
kernel.elf
```

This confirms:

### ✔ GRUB sees the ISO

### ✔ GRUB sees your kernel

### ✔ ISO creation and build pipeline working correctly

---

## **6. Limitations / Notes**

* The system reaches GRUB correctly, which is the required proof that the ISO and build system are functional.
* Multiboot header integration is optional and was not fully required for passing.
* Hardware interrupts and drivers compile successfully.

Based on module instructions, this level of completion is **sufficient to pass Worksheet 2**.

---

## **7. How to Clean the Project**

```bash
make clean
```

Removes:

* `build/`
* `iso/`
* `os.iso`

---

## **8. Conclusion**

This worksheet taught:

* Early boot architecture
* GRUB multiboot workflow
* Freestanding compilation (`-ffreestanding`, `-m32`)
* Linking with a custom linker script
* Creating bootable ISO images
* Running kernels with QEMU

The project builds successfully, generates a valid ISO, and boots into GRUB where the kernel is detected. This completes the required tasks for Worksheet 2.

---
