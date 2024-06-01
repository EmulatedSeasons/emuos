#include <mm/pmm.h>
#include <stdio.h>

PBitmap bitmap;

// Finds the biggest chunk of memory
static MMEntry find_mem_chunk(limine_memmap_response* memmap) {
    uint64_t highestmem = 0;
    size_t highestindex = 0;
    limine_memmap_entry* entries = memmap->entries[0];
    for (size_t i = 0; i < memmap->entry_count; i++) {
        if (entries[i].type == LIMINE_MEMMAP_USABLE && entries[i].length > highestmem) {
            highestmem = entries[i].length;
            highestindex = i;
        }
    }

    return MMEntry{
        entries[highestindex].base,
        entries[highestindex].length,
        MEM_USABLE
    };
}

void pmm_init(limine_memmap_response* memmap, uint64_t hhdm_offset) {
    MMEntry mem_entry = find_mem_chunk(memmap);

    // Round any numbers needed to 4K boudaries
    //mem_entry.base += 4096 - (mem_entry.base % 4096);
    mem_entry.length -= mem_entry.length % 4096;

    // setup bitmap at the beginning of the chunk
    bitmap.entry = mem_entry;
    bitmap.addr = (uint64_t*)(bitmap.entry.base + hhdm_offset);
    // divided by byte size since size is for iterating over the bitmap
    bitmap.size = mem_entry.length / 4096 / sizeof(uint64_t);

    printf("addr: %lx\nsize: %lx\n", (uint64_t)bitmap.addr, bitmap.size);
    printf("hhdm offset: %lx\n", hhdm_offset);

    // write test
    for (size_t i = 0; i < 100; i++) {
        bitmap.addr[i] = i;
        //printf("value %d: %lx\n", i, bitmap.addr[i]);
    }

    // read test
    for (size_t i = 0; i < bitmap.size; i++) {
        //printf("value %d: %lx\n");
    }
}

uint64_t get_phys_page() {

}