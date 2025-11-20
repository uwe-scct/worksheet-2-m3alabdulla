/* source/kernel.c - VGA + Serial output, minimal kernel */
typedef unsigned int uint32_t;
extern void fb_puts(const char *s);
extern void fb_putch(char c);

/* Serial helpers for COM1 (0x3F8) */
static inline void serial_io_wait(void) {
    volatile unsigned short *port = (volatile unsigned short *)0x80;
    (void)*port;
}
static inline void serial_write_char(char c) {
    volatile unsigned char *port = (volatile unsigned char *)0x3F8;
    *port = (unsigned char)c;
    serial_io_wait();
}
void serial_puts(const char *s) {
    while (*s) serial_write_char(*s++);
}
void serial_print_uint(unsigned int v) {
    char buf[12];
    int i = 0;
    if (v == 0) {
        serial_write_char('0');
        return;
    }
    while (v) {
        buf[i++] = '0' + (v % 10);
        v /= 10;
    }
    while (i--) serial_write_char(buf[i]);
}

/* VGA framebuffer helpers (text mode 80x25) */
#define VGA_ADDR ((volatile unsigned short *)0xB8000)
static volatile unsigned short *vga = VGA_ADDR;
static int cursor_pos = 0;

void fb_putch(char c) {
    unsigned short ch = (unsigned short)(0x07 << 8) | (unsigned char)c;
    if (c == '\n') {
        int row = cursor_pos / 80;
        row++;
        cursor_pos = row * 80;
    } else {
        vga[cursor_pos++] = ch;
    }
}
void fb_puts(const char *s) {
    while (*s) fb_putch(*s++);
}

/* small itoa-like function that prints decimal to VGA */
static void print_uint(uint32_t v) {
    char buf[12];
    int i = 0;
    if (v == 0) {
        fb_putch('0');
        return;
    }
    while (v) {
        buf[i++] = '0' + (v % 10);
        v /= 10;
    }
    while (i--) {
        fb_putch(buf[i]);
    }
}

/* Task: sum_of_three as required */
int sum_of_three(int a, int b, int c) {
    return a + b + c;
}

/* kernel entry called from loader.asm */
void kernel_main(void) {
    int s = sum_of_three(1, 2, 3); /* example */

    /* Print to VGA */
    fb_puts("Sum = ");
    print_uint((unsigned)s);
    fb_puts("\n");

    /* Also print the same to serial (ensures headless visibility) */
    serial_puts("Sum = ");
    serial_print_uint((unsigned)s);
    serial_puts("\n");

    /* Halt loop */
    for (;;) { __asm__ __volatile__("hlt"); }
}
