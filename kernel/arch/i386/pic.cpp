#include <io.h>
#include <arch/i386/pic.h>
#include <stdint.h>
#include <arch/i386/pic.h>

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_CMD PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_CMD PIC2
#define PIC2_DATA (PIC2 + 1)
#define PIC_READ_IRR 0x0a    // OCW3 irq ready next CMD read
#define PIC_READ_ISR 0x0b    // OCW3 irq service next CMD read

#define PIC_EOI 0x20

// pic reinitialization constants
#define ICW1_ICW4 0x01 // indicates icw4 will be present
#define ICW1_SINGLE 0x02 // single cascade mode
#define ICW1_INTERVAL4 0x04 // call address interval 4
#define ICW1_LEVEL 0x08 // level triggered (edge) mode
#define ICW1_INIT 0x10 // initialization

#define ICW4_8086 0x01 // 8086 mode
#define ICW4_AUTO 0x02 // auto (normal) eoi
#define ICW4_BUF_SLAVE 0x08 // buffered mode/slave
#define ICW4_BUF_MASTER 0x0C // buffered mode/master
#define ICW4_SFNM 0x10 // special fully nested (not)

void pic_send_eoi(unsigned char irq) {
    if (irq >= 8) {
        outb(PIC2_CMD, PIC_EOI);
    }
    outb(PIC1_CMD, PIC_EOI);
}

// offset 1 is for master, offset 2 is for slave
void pic_remap(int offset1, int offset2) {
    unsigned char a1, a2;

    a1 = inb(PIC1_DATA); // save masks
    a2 = inb(PIC2_DATA);

    outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4); // starts initialization in cascade mode
    io_wait();
    outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC1_DATA, offset1); // icw2: master and slave vector offset set
    io_wait();
    outb(PIC2_DATA, offset2);
    io_wait();
    outb(PIC1_DATA, 4); // tells master pic theres a slave pic at irq2
    io_wait();
    outb(PIC2_DATA, 2); // tells slave pic its cascade identity
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1); // restore masks
    outb(PIC2_DATA, a2);

    for (char i = 0; i < 15; i++)
	{
		irq_set_mask(i);
	}
}

void irq_set_mask(unsigned char irq) {
    uint16_t port;
    uint8_t value;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }
    value = inb(port) | (1 << irq);
    outb(port, value);
}

void irq_clear_mask(unsigned char irq) {
    uint16_t port;
    uint8_t value;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }
    value = inb(port) & ~(1 << irq);
    outb(port, value);
}