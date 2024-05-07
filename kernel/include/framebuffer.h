#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>

class Framebuffer {
    private:
    uint32_t* addr;
    const uint64_t width;
    const uint64_t height;

    public:
    Framebuffer(uint32_t* addr, const uint64_t width, const uint64_t height);
    void clear();
    void drawpixel(const uint64_t x, const uint64_t y, const uint32_t color);
};

#endif