#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "multiboot.h"
#include <arch/i386/tmtty.h>

// gets the memory map and stuff
extern "C" void multiboot_main(multiboot_info_t* mbd, uint32_t magic) {
    terminal_initialize();
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        printf("Invalid multiboot magic number\n");
        abort();
    }

    // checks bit 6 to see if there's a valid memory map
    if (!(mbd->flags >> 6 & 0x1)) {
        printf("Invalid GRUB memory map\n");
        abort();
    }

    // loop through memory map and display the values
    for (int i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
        multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*)(mbd->mmap_addr + i);

        printf("Start Addr: 0x%lx | Length: 0x%lx | Size: 0x%x | Type: %d\n", 
        mmmt->addr, mmmt->len, mmmt->size, mmmt->type);
        //printf("%lx,%x,%x,%d\n", mmmt->size, mmmt->size, mmmt->size, mmmt->size);
    }
}