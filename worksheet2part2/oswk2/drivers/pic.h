/* drivers/pic.h */
#ifndef PIC_H
#define PIC_H

void pic_remap(int offset1, int offset2);
void pic_ack(int irq);

#endif /* PIC_H */
