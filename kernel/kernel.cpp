#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <arch/x86_64/serial.h>
#include <limine.h>
#include <framebuffer.h>
#include <mm/pmm.h>
#include <arch/arch_init.h>

namespace {
__attribute__((used, section(".requests")))
volatile LIMINE_BASE_REVISION(3);
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
__attribute__((used, section(".requests")))
volatile limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
    .response = nullptr  
};
}

namespace {
__attribute__((used, section(".requests")))
volatile limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
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
[[noreturn]] void hcf() {
    asm("cli");
    for (;;) {
        asm("hlt");
    }
}
}

extern void (*__init_array[])();
extern void (*__init_array_end[])();

// linker symbols
uint64_t _kernel_begin;
uint64_t _kernel_end;

// this will cause problems later
uint64_t _hhdm_offset;

extern "C" void _start() {
    asm("cli");
    if (!LIMINE_BASE_REVISION_SUPPORTED) {
        hcf();
    }

    _hhdm_offset = hhdm_request.response->offset;

    arch_init();

    // initialize global constructors
    for (size_t i = 0; &__init_array[i] != __init_array_end; i++) {
        __init_array[i]();
    }

    // Ensure we got a framebuffer.
    if (framebuffer_request.response == nullptr
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    if (memmap_request.response == nullptr) {
        hcf();
    }

    // Fetch the first framebuffer.
    limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];

    fb_init((uint32_t*)framebuffer->address, framebuffer->pitch, framebuffer->height);
    draw_pixel(727, 727, 0x9528fd);
    draw_pixel(0, 0, 0xff0000);
    draw_pixel(framebuffer->width - 1, framebuffer->height - 1, 0x00ff00);

    printf("Actual framebuffer:\n");
    printf("Pitch %d\nWidth %d\nHeight %d\nbpp %d\nmem_model %d\n",
        framebuffer->pitch, framebuffer->width, framebuffer->height,
        framebuffer->bpp, framebuffer->memory_model);
    
    limine_memmap_response* memmap = memmap_request.response;
    for (int i = 0; i < memmap->entry_count; i++) {
        printf("base: %lx\nlength: %lx\ntype: %d\n\n", 
        memmap->entries[i]->base, memmap->entries[i]->length, memmap->entries[i]->type);
    }

    printf("hhdm offset: %lx\n", _hhdm_offset);
    pmm_init(memmap_request.response);

    uint64_t tester = page_alloc();
    printf("tester: %lx\n", tester);
    page_free(tester);
    tester = page_alloc();
    printf("tester: %lx\n", tester);
    uint64_t tester2 = page_alloc();
    printf("tester 2: %lx\n", tester2);
    
    // We're done, just hang...
    hcf();
}