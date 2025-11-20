; drivers/load_idt.asm
; provide load_idt(void *ptr) which does: lidt [ptr]
; NASM / ELF

global load_idt
section .text

; cdecl: argument ptr at [esp+4]
load_idt:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]    ; eax = pointer to IDT pointer (limit:base struct)
    lidt [eax]          ; load IDT from memory pointed by eax
    pop ebp
    ret
