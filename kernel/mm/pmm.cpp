#include <mm/pmm.h>
#include <kernel.h>
#include <stdio.h>

#define BIT_WIDTH 64

Bitmap* head;

void pmm_init(limine_memmap_response* memmap) {
    Bitmap* current = nullptr;
    Bitmap* tail = nullptr;
    bool first = true;

    for (size_t i = 0; i < memmap->entry_count; i++) {
        if (memmap->entries[i]->type == LIMINE_MEMMAP_USABLE) {
            // sets bitmap struct pointer address
            current = (Bitmap*)(memmap->entries[i]->base + _hhdm_offset);

            // sets up bitmap
            current->address = (uint64_t*)(memmap->entries[i]->base + sizeof(Bitmap) + _hhdm_offset);

            current->limit = memmap->entries[i]->length;

            // almost always will be less pages than the full space
            // since it doesn't account for remainders (yet)
            // length / page size / bit width
            current->length = memmap->entries[i]->length / 0x1000uL / BIT_WIDTH;
            if (current->length < 1) {
                continue;
            }
            // printf("len %lx\n", current->length);

            // allocate memory for the bitmaps themselves
            // printf("test %lx\n", current->limit / 0x40000uL / BIT_WIDTH);
            for (size_t i = 0; i < (current->limit / 0x1000uL / BIT_WIDTH / BIT_WIDTH) + 1; i++) {
                for (size_t j = 0; j < BIT_WIDTH; j++) {
                    // shifts bits off the right and masks the rest out to check for a 0
                    if (!((current->address[i] >> j) & 0x1uL)) {
                        current->address[i] |= 0x1uL << j;
                        // printf("init alloc: %lx\n", ((i * BIT_WIDTH) + j) * 0x1000uL + (uint64_t)current - _hhdm_offset);
                        break;
                    }
                }
            }

            if (first) {
                current->next = nullptr;
                tail = current;
                head = current;
                first = false;
            } 
            else {
                tail->next = current;
                tail = current;
            }
        }
    }

    tail->next = nullptr;

    printf("Initialized PMM\n");
}

uint64_t palloc() {
    Bitmap* current = head;

    while (true) {
        for (size_t i = 0; i < current->length; i++) {
            //if ((current->address[i] & UINT64_MAX) != UINT64_MAX) {
                for (size_t j = 0; j < BIT_WIDTH; j++) {
                    // shifts bits off the right and masks the rest out to check for a 0
                    if (!((current->address[i] >> j) & 0x1uL)) {
                        current->address[i] |= 0x1uL << j;
                        // printf("addr: %lx\n", current + j);
                        return ((i * BIT_WIDTH) + j) * 0x1000uL + (uint64_t)current - _hhdm_offset;
                    }
                }
            //}
        }

        if (current->next == nullptr) {
            return NULL;
        }
        current = current->next;
    }
}

void pfree(uint64_t page) {
    Bitmap* current = head;
    int offset;

    while (true) {
        if (page + _hhdm_offset >= (uint64_t)current) {
            if (page + _hhdm_offset <= current->limit + (uint64_t)current) {
                break;
            }
        }
        current = current->next;
    }
    
    // Gets the starting number of the mem region page to get the correct bit number
    // start 4D000, freeing 4E000, offset D - E = 1 = bit in bitmap
    offset = ((uint64_t)current / 0x1000uL) & 0xFuL; 
    // divided by the amount of address space each index can hold
    // should probably put that in its own variable/define later
    uint64_t index = (page - ((uint64_t)current - _hhdm_offset)) / 0x40000uL;
    int bit = page / 0x1000uL % BIT_WIDTH - offset;

    // printf("before: %lx, ", current->address[index]);
    current->address[index] &= ~(0x1uL << bit);
    // printf("after: %lx\n", current->address[index]);
}