#include "gdt.h"
#include "idt.h"

void arch_init() {
    // setup gdt
    SegDesc segs[5];
    GDTR gdtr{sizeof(struct SegDesc) * 5 - 1, (uint64_t)&segs};

    gdt_entry(&segs[0], 0, 0, 0, 0); // null desc
    gdt_entry(&segs[1], 0, 0xFFFFF, 0x9A, 0xA); // kcode
    gdt_entry(&segs[2], 0, 0xFFFFF, 0x92, 0xA); // kdata
    gdt_entry(&segs[3], 0, 0xFFFFF, 0xFA, 0xA); // ucode
    gdt_entry(&segs[4], 0, 0xFFFFF, 0xF2, 0xA); // udata
    
    load_gdt(&gdtr);
    reload_segments();

    setup_idt();
}