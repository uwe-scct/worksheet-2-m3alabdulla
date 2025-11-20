/* drivers/keyboard.h */
#ifndef KEYBOARD_H
#define KEYBOARD_H

/* initialize keyboard (if needed) */
void keyboard_init(void);

/* Called by C code to process key events (non-blocking) */
int kb_getchar(void);    /* -1 if none, else ascii */

/* Called by IRQ handler (C callback) */
void keyboard_interrupt_c(void);

#endif /* KEYBOARD_H */
