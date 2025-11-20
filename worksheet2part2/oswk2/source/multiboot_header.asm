; Minimal Multiboot header (NASM syntax) for GRUB Multiboot 0.6

section .multiboot
    align 4
    dd 0x1BADB002          ; magic
    dd 0x00010003          ; flags: 0x00010003 -> align, memory info, mods, and a few common flags
    dd -(0x1BADB002 + 0x00010003) ; checksum: magic + flags + checksum == 0
