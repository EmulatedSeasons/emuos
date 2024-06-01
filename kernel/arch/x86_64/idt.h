#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct IDTR {
    uint16_t size;
    uint64_t offset;
}__attribute__((packed));

struct IdtEntry {
    uint16_t offset_low;
    uint16_t seg_sel;
    uint8_t ist;
    uint8_t type_attributes;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t reserved;
}__attribute__((packed));

void idt_entry(int num, uint64_t* offset, uint16_t sel, uint8_t ist, uint8_t type_attributes);
void setup_idt();

#endif