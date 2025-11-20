/* drivers/interrupts.c */
#include "interrupts.h"
#include "pic.h"
#include <stdint.h>

/* IDT structures */
struct IDTDescriptor {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  type_attr;
    uint16_t offset_high;
} __attribute__((packed));

struct IDTPointer {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

/* we will create an IDT of 256 entries in bss */
static struct IDTDescriptor idt[256];
extern void load_idt(void *ptr);   /* implemented in assembly (see below) */

/* external IRQ1 handler in assembly */
extern void irq1_handler(void);

/* helper to set single IDT entry */
static void set_idt_entry(int vec, void (*handler)(void)) {
    uint32_t handler_addr = (uint32_t) handler;
    idt[vec].offset_low = handler_addr & 0xFFFF;
    idt[vec].selector   = 0x08;    /* code segment selector */
    idt[vec].zero       = 0;
    idt[vec].type_attr  = 0x8E;    /* present, ring 0, 32-bit interrupt gate */
    idt[vec].offset_high = (handler_addr >> 16) & 0xFFFF;
}

void interrupts_install_idt(void) {
    int i;
    for (i = 0; i < 256; ++i) {
        set_idt_entry(i, 0);
    }

    /* Remap PICs so IRQ0-15 map to 0x20-0x2F, keyboard IRQ is 1 -> vector 0x21 */
    pic_remap(0x20, 0x28);

    /* Set keyboard handler for vector 0x21 */
    set_idt_entry(0x21, irq1_handler);

    /* load idt */
    struct IDTPointer p;
    p.limit = (sizeof(idt) - 1);
    p.base = (uint32_t) &idt;
    load_idt(&p);
}

/* wrappers for enabling/disabling interrupts */
void enable_interrupts(void) {
    __asm__ volatile ("sti");
}
void disable_interrupts(void) {
    __asm__ volatile ("cli");
}
