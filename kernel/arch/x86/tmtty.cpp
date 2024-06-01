#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <arch/i386/tmtty.h>

#include "vga.h"

/* Text mode tty */

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_clear(void) {
    terminal_row = 0;
    terminal_column = 0;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t i = y * VGA_WIDTH + x;
            terminal_buffer[i] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_initialize(void) {
    terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_buffer = VGA_MEMORY;
    terminal_clear();
}

void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t i = y * VGA_WIDTH + x;
    terminal_buffer[i] = vga_entry(c, color);
}

void terminal_scroll() {
    memmove(VGA_MEMORY, (VGA_MEMORY + VGA_WIDTH), (VGA_HEIGHT * VGA_WIDTH) - VGA_WIDTH);
	memset(VGA_MEMORY + (VGA_WIDTH * VGA_HEIGHT) - VGA_WIDTH, 0x0, VGA_WIDTH * 2);
	--terminal_row;
}

void terminal_putchar(char c) {
    unsigned char uc = c;
    if (uc == '\n') {
        terminal_column = 0;
        if (terminal_row++ == VGA_HEIGHT)
			terminal_scroll();
        return;
    }
    terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
    
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_scroll();
        }
    }
}

void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}