/* drivers/keyboard.c - keyboard ISR + small scan->ASCII mapping
 * Uses drivers/io.h for inb/outb
 */

#include "keyboard.h"
#include "pic.h"
#include "io.h"      /* provides inb/outb as inline */
#include <stdint.h>

/* keyboard data port */
#define KEYBOARD_DATA_PORT 0x60

/* small circular buffer */
#define INPUT_BUFFER_SIZE 256
static volatile unsigned char input_buffer[INPUT_BUFFER_SIZE];
static volatile int buffer_head = 0;
static volatile int buffer_tail = 0;

int kb_getchar(void) {
    if (buffer_head == buffer_tail) return -1;
    int c = input_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % INPUT_BUFFER_SIZE;
    return c;
}

/* Basic scan->ASCII mapping for common keys (partial, enough for demo) */
static unsigned char scan_to_ascii(unsigned char scan) {
    switch (scan) {
        case 0x02: return '1';
        case 0x03: return '2';
        case 0x04: return '3';
        case 0x05: return '4';
        case 0x06: return '5';
        case 0x07: return '6';
        case 0x08: return '7';
        case 0x09: return '8';
        case 0x0A: return '9';
        case 0x0B: return '0';
        case 0x0C: return '-';
        case 0x0D: return '=';
        case 0x10: return 'q';
        case 0x11: return 'w';
        case 0x12: return 'e';
        case 0x13: return 'r';
        case 0x14: return 't';
        case 0x15: return 'y';
        case 0x16: return 'u';
        case 0x17: return 'i';
        case 0x18: return 'o';
        case 0x19: return 'p';
        case 0x1E: return 'a';
        case 0x1F: return 's';
        case 0x20: return 'd';
        case 0x21: return 'f';
        case 0x22: return 'g';
        case 0x23: return 'h';
        case 0x24: return 'j';
        case 0x25: return 'k';
        case 0x26: return 'l';
        case 0x2C: return 'z';
        case 0x2D: return 'x';
        case 0x2E: return 'c';
        case 0x2F: return 'v';
        case 0x30: return 'b';
        case 0x31: return 'n';
        case 0x32: return 'm';
        case 0x39: return ' '; /* space */
        case 0x1C: return '\n'; /* Enter */
        case 0x0E: return '\b'; /* Backspace */
        default: return 0;
    }
}

/* Called from assembly IRQ handler */
void keyboard_interrupt_c(void) {
    unsigned char scancode = inb(KEYBOARD_DATA_PORT);
    /* Ignore break codes (high bit set) */
    if (scancode & 0x80) {
        /* ignore break codes */
    } else {
        unsigned char ch = scan_to_ascii(scancode);
        if (ch) {
            int next = (buffer_head + 1) % INPUT_BUFFER_SIZE;
            if (next != buffer_tail) {
                input_buffer[buffer_head] = ch;
                buffer_head = next;
            }
        }
    }
    /* Acknowledge PIC for IRQ1 (keyboard) */
    pic_ack(1);
}
