#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>

void fb_init(uint32_t* addr, uint32_t width, uint32_t height);
void fb_clear();
void draw_pixel(const uint32_t x, const uint32_t y, const uint32_t color);
void draw_bitmap(const uint32_t* bitmap, const uint32_t x_pos, const uint32_t y_pos, const uint32_t width, const uint32_t height);

#endif