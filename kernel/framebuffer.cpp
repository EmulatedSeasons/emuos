#include <framebuffer.h>
#include <stdint.h>
#include <stddef.h>

Framebuffer::Framebuffer(uint32_t* addr, const uint64_t width, const uint64_t height) 
    : addr {addr}, width {width}, height {height} {
    clear();
}

void Framebuffer::clear() {
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            const size_t i = y * width + x;
            addr[i] = 0x000000;
        }
    }
}

void Framebuffer::drawpixel(const uint64_t x, const uint64_t y, const uint32_t color) {
    const size_t pos = y * width + x;
    addr[pos] = color;
}