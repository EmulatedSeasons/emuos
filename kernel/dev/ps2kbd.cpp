#include <stdint.h>
//#include <stdio.h>
//#include <arch/i386/pic.h>
#include <io.h>

// Keycodes are sorted from left to right, up to down starting with esc
// 0x80 is reserved for nonexistant scancodes
// used for scan code set 2
// constexpr uint8_t scancodes[] = {
//     9, 0x80, 5, 3, 1, 2, 12, 0x80,
//     10, 8, 6, 4, 27, 
// };

// void keyboard_init() {
//     irq_clear_mask(1);
//     outb(0x60, 0xF4);
//     inb(0x60);
// }

// void keyboard_write(uint8_t scancode) {
//     printf("a\n");
//     /*for (int i = 0; i < 7; i++) {
//         inputbuffer |= inb(PS2_DATA) << (i * 8); // lsb is the first scan code
//     }*/
// }