#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct GDTR {
    uint16_t size;
    uint64_t offset;
}__attribute__((packed));

struct SegDesc {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t flag_limit_high;
    uint8_t base_high;
}__attribute__((packed));

// tss segment descriptor is larger than the normal ones
struct TssSegDesc {
    uint16_t limit_low;
    uint16_t base1; // lowest base
    uint8_t base2;
    uint8_t access;
    uint8_t flag_limit_high; // both in the same 8 bits
    uint8_t base3;
    uint32_t base4; // highest base
    uint32_t reserved;
}__attribute__((packed));

void gdt_entry(SegDesc* seg, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);
void gdt_tss_entry(TssSegDesc* seg, uint64_t base, uint32_t limit, uint8_t access, uint8_t flags);

extern "C" void load_gdt(GDTR*);
extern "C" void reload_segments();

#endif