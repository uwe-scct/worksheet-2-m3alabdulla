/* drivers/io.h
 * Simple I/O port helpers used by the worksheet code.
 * Put this file in the drivers/ directory next to pic.c
 */

#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

/* Basic sized types used across the OS - minimal set */
#include <stdint.h>
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

/* inb: read byte from I/O port */
static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0"
                      : "=a"(ret)
                      : "Nd"(port));
    return ret;
}

/* outb: write byte to I/O port */
static inline void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1"
                      :
                      : "a"(val), "Nd"(port));
}

/* inw: read 16-bit word from I/O port (optional) */
static inline unsigned short inw(unsigned short port) {
    unsigned short ret;
    __asm__ volatile ("inw %1, %0"
                      : "=a"(ret)
                      : "Nd"(port));
    return ret;
}

/* outw: write 16-bit word to I/O port (optional) */
static inline void outw(unsigned short port, unsigned short val) {
    __asm__ volatile ("outw %0, %1"
                      :
                      : "a"(val), "Nd"(port));
}

#endif /* INCLUDE_IO_H */
