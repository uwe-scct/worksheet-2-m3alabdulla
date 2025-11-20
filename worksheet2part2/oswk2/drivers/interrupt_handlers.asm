; drivers/interrupt_handlers.asm
; Simple IRQ handler glue: save registers, call C handler, restore, iret
; NASM/ELF

global irq1_handler
extern keyboard_interrupt_c

section .text

irq1_handler:
    pusha                      ; save general registers
    pushad                     ; alternative to pusha (but using both is harmless on i386? we will use only pusha)
    ; call C function to process keyboard
    call keyboard_interrupt_c
    ; restore registers
    popad
    popa
    iret
