#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <arch/i386/tmtty.h>
#include <arch/i386/ps2_controller.h>
#include <arch/i386/pic.h>
#include <io.h>

extern "C" void exception_handler(int e) {
    //unsigned char errorcode[2]{((unsigned char) e) + 48};
    //terminal_initialize();
	printf("Exception\n");
}

extern "C" void ps2_handler() {
    printf("b");
    inb(0x60);
    printf("a");

    pic_send_eoi(1);
}