#include <stdint.h>
#include <stdio.h>
#include <arch/i386/pic.h>
#include <io.h>

void keyboard_init() {
    irq_clear_mask(1);
    outb(0x60, 0xF4);
    inb(0x60);
}