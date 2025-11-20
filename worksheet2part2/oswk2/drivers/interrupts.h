/* drivers/interrupts.h */
#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

void interrupts_install_idt(void);
void enable_interrupts(void);
void disable_interrupts(void);

#endif /* INTERRUPTS_H */
