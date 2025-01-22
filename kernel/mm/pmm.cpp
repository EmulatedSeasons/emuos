#include <mm/pmm.h>
#include <kernel.h>
#include <stdio.h>

#define PAGE_SIZE 0x1000ul
// makes sure double frees aren't happening (probably)
#define PMM_MAGIC_NUM 0xE1321ul

struct freelist {
    freelist* next = nullptr;
    uint64_t magic_num;
};

freelist* head = nullptr;

void pmm_init(limine_memmap_response* memmap) {

    for (int i = 0; i < memmap->entry_count; i++) {
        // only maps usable entries
        if (memmap->entries[i]->type == LIMINE_MEMMAP_USABLE) {
            printf("%d\n", i);
            // iterates through all addresses
            for (uint64_t addr = memmap->entries[i]->base; addr < (memmap->entries[i]->length + memmap->entries[i]->base); addr += PAGE_SIZE) {
                freelist* temp = (freelist*)(addr + _hhdm_offset);
                temp->magic_num = PMM_MAGIC_NUM;
                if (head) {
                    temp->next = head;
                    head = temp;
                } else {
                    temp->next = nullptr;
                    head = temp;
                }
            }
        }
    }

    printf("Initialized PMM\n");
}

uint64_t page_alloc() {
    freelist* alloced = head;
    alloced->magic_num = 0;
    head = head->next;
    return (uint64_t)alloced;
}

void page_free(uint64_t page) {
    freelist* freed = (freelist*)page;

    // double free check
    if (freed->magic_num == PMM_MAGIC_NUM) {
        return;
    }

    freed->next = head;
    freed->magic_num = PMM_MAGIC_NUM;
    head = freed;
}