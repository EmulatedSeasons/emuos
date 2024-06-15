#include <mm/pmm.h>
#include <kernel.h>
#include <stdio.h>

// Finds biggest memory chunk to place the bitmap
static int find_biggest_chunk(limine_memmap_response* memmap) {
    int biggestmem = 0;
    int biggestindex = 0;
    for (size_t i = 0; i < memmap->entry_count; i++) {
        if (memmap->entries[i]->length > biggestmem) {
            biggestmem = memmap->entries[i]->length;
            biggestindex = i;
        }
    }

    return biggestindex;
}

static MemRegion** setup_meminfo(limine_memmap_response* memmap, int chunk) {
    const uint64_t* basemem = (uint64_t*)memmap->entries[chunk]->base;
    int use_count = 1; // for placing structures in the right location

    for (size_t i = 0; i < memmap->entry_count; i++) {
        if (memmap->entries[i]->type == LIMINE_MEMMAP_USABLE) {
            
        }
    }
}

void pmm_init(limine_memmap_response* memmap) {
    int chunk = find_biggest_chunk(memmap);

    setup_meminfo(memmap, chunk);

    printf("Initialized PMM\n");
}

void* palloc() {

}

void pfree(void* page) {
    
}