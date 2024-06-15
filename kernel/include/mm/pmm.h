#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stddef.h>
#include <limine.h>

struct MemRegion {
    uint64_t base;
    uint64_t length;
    uint64_t bitmap_pos; // where the region starts on the bitmap index
};

struct BitmapInfo {
    uint64_t* address;
    uint64_t ind_length;
    int region_count;
    MemRegion* regions[];
};

void pmm_init(limine_memmap_response* memmap);
void* palloc();
void pfree(void* page);

#endif