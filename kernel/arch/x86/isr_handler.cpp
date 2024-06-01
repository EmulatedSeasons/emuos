#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <arch/i386/tmtty.h>
#include <arch/i386/ps2_controller.h>
#include <dev/ps2kbd.h>
#include <arch/i386/pic.h>
#include <io.h>

extern "C" void exception_handler(int e) {
    //unsigned char errorcode[2]{((unsigned char) e) + 48};
    //terminal_initialize();
	printf("Exception %x\n", e);
}

#define PS2_DATA 0x60
extern "C" void ps2_handler() {
    keyboard_write(inb(PS2_DATA));

    pic_send_eoi(1);
}