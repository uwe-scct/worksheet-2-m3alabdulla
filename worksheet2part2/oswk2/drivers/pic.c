/* drivers/pic.c - PIC remap + acknowledge routines
 * Uses inb/outb from drivers/io.h
 */

#include "pic.h"
#include "io.h"
#include <stdint.h>

/* PIC ports */
#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

/* Initialization Control Word bits */
#define ICW1_INIT  0x11
#define ICW4_8086  0x01

void pic_remap(int offset1, int offset2) {
    unsigned char a1, a2;

    /* save masks */
    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);

    /* starts the init sequence (in cascade mode) */
    outb(PIC1_COMMAND, ICW1_INIT);
    outb(PIC2_COMMAND, ICW1_INIT);

    /* Set vector offsets */
    outb(PIC1_DATA, (unsigned char)offset1);
    outb(PIC2_DATA, (unsigned char)offset2);

    /* Tell Master PIC that there is a slave PIC at IRQ2 (0000 0100) */
    outb(PIC1_DATA, 4);
    /* Tell Slave PIC its cascade identity (0000 0010) */
    outb(PIC2_DATA, 2);

    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    /* restore saved masks (keeps previous IRQ mask state) */
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

void pic_ack(int irq) {
    if (irq >= 8) {
        outb(PIC2_COMMAND, 0x20);
    }
    outb(PIC1_COMMAND, 0x20);
}
