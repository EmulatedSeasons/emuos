#include <stdint.h>
#include "idt.h"

extern "C" void isr0();
extern "C" void isr1();
extern "C" void isr2();
extern "C" void isr3();
extern "C" void isr4();
extern "C" void isr5();
extern "C" void isr6();
extern "C" void isr7();
extern "C" void isr8();
extern "C" void isr9();
extern "C" void isr10();
extern "C" void isr11();
extern "C" void isr12();
extern "C" void isr13();
extern "C" void isr14();
extern "C" void isr15();
extern "C" void isr16();
extern "C" void isr17();
extern "C" void isr18();
extern "C" void isr19();
extern "C" void isr20();
extern "C" void isr21();
extern "C" void isr22();
extern "C" void isr23();
extern "C" void isr24();
extern "C" void isr25();
extern "C" void isr26();
extern "C" void isr27();
extern "C" void isr28();
extern "C" void isr29();
extern "C" void isr30();
extern "C" void isr31();

IdtEntry idt[256];
IDTR idtr;

void idt_entry(int num, uint64_t offset, uint16_t sel, uint8_t ist, uint8_t type_attributes) {
    idt[num].offset_low = (uint64_t)offset & 0xFFFF;
    idt[num].offset_mid = ((uint64_t)offset & 0xFFFF0000) >> 16;
    idt[num].offset_high = ((uint64_t)offset & 0xFFFFFFFF00000000) >> 32;

    idt[num].seg_sel = sel;

    idt[num].ist = ist & 0b111;

    idt[num].type_attributes = type_attributes;

    idt[num].reserved = 0;
}

extern "C" void load_idt(IDTR*);

void setup_idt() {
    idtr.size = (sizeof(struct IDTR) * 256);
    idtr.offset = (uint64_t)idt;

    idt_entry(0, (uint64_t)isr0, 0x08, 0, 0x8E);
    idt_entry(1, (uint64_t)isr1, 0x08, 0, 0x8E);
    idt_entry(2, (uint64_t)isr2, 0x08, 0, 0x8E);
    idt_entry(3, (uint64_t)isr3, 0x08, 0, 0x8E);
    idt_entry(4, (uint64_t)isr4, 0x08, 0, 0x8E);
    idt_entry(5, (uint64_t)isr5, 0x08, 0, 0x8E);
    idt_entry(6, (uint64_t)isr6, 0x08, 0, 0x8E);
    idt_entry(7, (uint64_t)isr7, 0x08, 0, 0x8E);
    idt_entry(8, (uint64_t)isr8, 0x08, 0, 0x8E);
    idt_entry(9, (uint64_t)isr9, 0x08, 0, 0x8E);
    idt_entry(10, (uint64_t)isr10, 0x08, 0, 0x8E);
    idt_entry(11, (uint64_t)isr11, 0x08, 0, 0x8E);
    idt_entry(12, (uint64_t)isr12, 0x08, 0, 0x8E);
    idt_entry(13, (uint64_t)isr13, 0x08, 0, 0x8E);
    idt_entry(14, (uint64_t)isr14, 0x08, 0, 0x8E);
    idt_entry(15, (uint64_t)isr15, 0x08, 0, 0x8E);
    idt_entry(16, (uint64_t)isr16, 0x08, 0, 0x8E);
    idt_entry(17, (uint64_t)isr17, 0x08, 0, 0x8E);
    idt_entry(18, (uint64_t)isr18, 0x08, 0, 0x8E);
    idt_entry(19, (uint64_t)isr19, 0x08, 0, 0x8E);
    idt_entry(20, (uint64_t)isr20, 0x08, 0, 0x8E);
    idt_entry(21, (uint64_t)isr21, 0x08, 0, 0x8E);
    idt_entry(22, (uint64_t)isr22, 0x08, 0, 0x8E);
    idt_entry(23, (uint64_t)isr23, 0x08, 0, 0x8E);
    idt_entry(24, (uint64_t)isr24, 0x08, 0, 0x8E);
    idt_entry(25, (uint64_t)isr25, 0x08, 0, 0x8E);
    idt_entry(26, (uint64_t)isr26, 0x08, 0, 0x8E);
    idt_entry(27, (uint64_t)isr27, 0x08, 0, 0x8E);
    idt_entry(28, (uint64_t)isr28, 0x08, 0, 0x8E);
    idt_entry(29, (uint64_t)isr29, 0x08, 0, 0x8E);
    idt_entry(30, (uint64_t)isr30, 0x08, 0, 0x8E);
    idt_entry(31, (uint64_t)isr31, 0x08, 0, 0x8E);

    load_idt(&idtr);
}