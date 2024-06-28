#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stddef.h>
#include <limine.h>

struct Bitmap {
    Bitmap* next;
    uint64_t length;
    uint64_t limit;
    uint64_t* address;
};

void pmm_init(limine_memmap_response* memmap);
uint64_t palloc();
void pfree(uint64_t page);

#endif