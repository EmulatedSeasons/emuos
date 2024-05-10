#include <stdint.h>
#include "gdt.h"

void gdt_entry(SegDesc* seg, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    // setting each part of the base by masking the part being set and shifting it to the right position
    seg->base_low = base & 0xFFFF; 
    seg->base_mid = (base & 0xFF0000) >> 16;
    seg->base_high = (base & 0xFF000000) >> 24;

    seg->limit_low = limit & 0xFFFF;
    seg->flag_limit_high = (limit & 0xF0000) >> 16;

    // shifts flags to the right position and ors it with limit
    seg->flag_limit_high |= (flags & 0xF) << 4;

    seg->access = access;
}

void gdt_tss_entry(TssSegDesc* seg, uint64_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    // setting each part of the base by masking the part being set and shifting it to the right position
    seg->base1 = base & 0xFFFF; 
    seg->base2 = (base & 0xFF0000) >> 16;
    seg->base3 = (base & 0xFF000000) >> 24;
    seg->base4 = (base & 0xFFFFFFFF00000000) >> 32;

    seg->limit_low = limit & 0xFFFF;
    seg->flag_limit_high = (limit & 0xF0000) >> 16;

    // shifts flags to the right position and ors it with limit
    seg->flag_limit_high |= (flags & 0xF) << 4;

    seg->access = access;
    seg->reserved = 0;
}