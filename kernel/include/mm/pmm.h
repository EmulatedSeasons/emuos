#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stddef.h>
#include <limine.h>

void pmm_init(limine_memmap_response* memmap);
uint64_t page_alloc();
void page_free(uint64_t page);

#endif