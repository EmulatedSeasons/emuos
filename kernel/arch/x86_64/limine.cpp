#include <stdint.h>
#include <stddef.h>
#include "limine.h"

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
    if (!LIMINE_BASE_REVISION_SUPPORTED) {
        hcf();
    }

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
    for (size_t i = 0; i < 100; i++) {
        volatile uint32_t *fb_ptr = static_cast<volatile uint32_t *>(framebuffer->address);
        fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xffffff;
    }



    // We're done, just hang...
    hcf();
}