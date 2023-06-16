#ifndef i386_PIC_H
#define i386_PIC_H

extern "C" void pic_remap(int, int);
void pic_send_eoi(unsigned char);
void irq_set_mask(unsigned char);
void irq_clear_mask(unsigned char);

#endif