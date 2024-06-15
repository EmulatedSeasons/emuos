#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stddef.h>
#include <limine.h>

struct pmm_list_node {
    pmm_list_node* next;
};

void pmm_init(limine_memmap_response* memmap);
void* palloc();
void pfree(void* page);

#endif