#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>

void fb_init(uint32_t* addr, uint64_t width, uint64_t height);
void fb_clear();
void draw_pixel(const uint64_t x, const uint64_t y, const uint32_t color);
void draw_bitmap(const uint32_t* bitmap, const uint64_t x_pos, const uint64_t y_pos, const uint64_t width, const uint64_t height);

#endif