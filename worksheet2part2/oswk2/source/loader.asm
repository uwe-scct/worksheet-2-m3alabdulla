; source/loader.asm
; Minimal Multiboot2 header + tiny loader that calls kernel_main.
; NASM (elf) format.

; place the multiboot header in its own section (aligned)
section .multiboot
align 8
mb_header_start:
    dd 0xE85250D6        ; multiboot2 magic
    dd 0                 ; architecture (0 = i386)
    dd mb_header_end - mb_header_start  ; header length (bytes)
    dd - (0xE85250D6 + 0 + (mb_header_end - mb_header_start)) ; checksum

    ; minimal tag: end tag (type=0, size=8)
    dd 0                 ; tag type = END
    dd 8                 ; tag size = 8

mb_header_end:

; now the code
section .text
global loader
extern kernel_main

align 4
loader:
    ; move a magic value to EAX for debug (optional)
    mov eax, 0xCAFEBABE

    ; call the C kernel entry
    call kernel_main

.hang:
    jmp .hang
