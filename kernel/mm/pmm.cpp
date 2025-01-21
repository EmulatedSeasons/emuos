#include <mm/pmm.h>
#include <kernel.h>
#include <stdio.h>

struct freelist {
    freelist* next = nullptr;
};

freelist* head = nullptr;

void pmm_init(limine_memmap_response* memmap) {

    for (int i = 0; i < memmap->entry_count; i++) {
        // only maps usable entries
        if (memmap->entries[i]->type == LIMINE_MEMMAP_USABLE) {
            printf("%d\n", i);
            // iterates through all addresses
            for (uint64_t addr = memmap->entries[i]->base; addr < (memmap->entries[i]->length + memmap->entries[i]->base); addr += 0x1000ul) {
                freelist* temp = (freelist*)(addr + _hhdm_offset);
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
    head = head->next;
    return (uint64_t)alloced;
}

void page_free(uint64_t page) {
    freelist* freed = (freelist*)page;
    freed->next = head;
    head = freed;
}