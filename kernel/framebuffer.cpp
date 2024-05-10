#include <framebuffer.h>
#include <stdint.h>
#include <stddef.h>

uint32_t* fb_addr;
uint64_t fb_width;
uint64_t fb_height;

void fb_init(uint32_t* addr, uint64_t width, uint64_t height) {
    fb_addr = addr;
    fb_width = width;
    fb_height = height;
    fb_clear();
}

void fb_clear() {
    for (size_t y = 0; y < fb_height; y++) {
        for (size_t x = 0; x < fb_width; x++) {
            const size_t i = y * fb_width + x;
            fb_addr[i] = 0x000000;
        }
    }
}

void draw_pixel(const uint64_t x, const uint64_t y, const uint32_t color) {
    const size_t pos = y * fb_width + x;
    fb_addr[pos] = color;
}

void draw_bitmap(const uint32_t* bitmap, const uint64_t x_pos, const uint64_t y_pos, const uint64_t width, const uint64_t height) {
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            const size_t i = y * width + x;
            draw_pixel(x_pos + x, y_pos + y, bitmap[i]);
        }
    }    
}