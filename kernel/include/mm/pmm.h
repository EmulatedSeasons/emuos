#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stddef.h>
#include <limine.h>

enum mm_type {
    MEM_USABLE,
    MEM_RESERVED
};

struct MMEntry {
    uint64_t base;
    uint64_t length;
    mm_type type;
};

struct PBitmap {
    uint64_t* addr;
    size_t size;
    MMEntry entry;
};

void pmm_init(limine_memmap_response* memmap, uint64_t hhdm_offset);

#endif