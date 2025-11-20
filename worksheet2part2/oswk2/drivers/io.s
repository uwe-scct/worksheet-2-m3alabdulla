; drivers/io.s
; Simple inb implementation (cdecl)
; NASM / ELF

global inb

section .text

inb:
    ; cdecl: port argument at [esp+4]
    mov dx, [esp+4]
    in al, dx
    movzx eax, al
    ret
