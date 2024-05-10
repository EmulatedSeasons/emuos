#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <arch/x86_64/serial.h>
#include "limine.h"
#include "gdt.h"
#include "idt.h"
#include <framebuffer.h>

namespace {
__attribute__((used, section(".requests")))
volatile LIMINE_BASE_REVISION(2);
}

namespace {
__attribute__((used, section(".requests")))
volatile limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = nullptr
};
}

namespace {
__attribute__((used, section(".requests_start_marker")))
volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_marker")))
volatile LIMINE_REQUESTS_END_MARKER;
}

namespace {
void hcf() {
    asm("cli");
    for (;;) {
        asm("hlt");
    }
}
}

extern void (*__init_array[])();
extern void (*__init_array_end[])();

extern "C" void kernel_main();

extern "C" void _start() {
    asm("cli");
    if (!LIMINE_BASE_REVISION_SUPPORTED) {
        hcf();
    }

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

    // initialize global constructors
    for (size_t i = 0; &__init_array[i] != __init_array_end; i++) {
        __init_array[i]();
    }

    // Ensure we got a framebuffer.
    if (framebuffer_request.response == nullptr
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    // Fetch the first framebuffer.
    limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    // for (size_t i = 0; i < 100; i++) {
    //     volatile uint32_t *fb_ptr = static_cast<volatile uint32_t *>(framebuffer->address);
    //     fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xffffff;
    // }

    asm("xchgw %bx, %bx");
    fb_init((uint32_t*)framebuffer->address, framebuffer->width, framebuffer->height);
    draw_pixel(727, 727, 0x9528fd);
    draw_pixel(0, 0, 0xff0000);
    draw_pixel(1919, 1079, 0x00ff00);
    // for (size_t i = 0; i < framebuffer->mode_count; i++) {
    //     printf("Mode %d\n", i);
    //     printf("Pitch %d\nWidth %d\nHeight %d\nbpp %d\nmem_model %d\n",
    //     framebuffer->modes[i]->pitch, framebuffer->modes[i]->width, framebuffer->modes[i]->height,
    //     framebuffer->modes[i]->bpp, framebuffer->modes[i]->memory_model);
    // }

    printf("Actual framebuffer:\n");
    printf("Pitch %d\nWidth %d\nHeight %d\nbpp %d\nmem_model %d\n",
        framebuffer->pitch, framebuffer->width, framebuffer->height,
        framebuffer->bpp, framebuffer->memory_model);

    kernel_main();

    // We're done, just hang...
    hcf();
}