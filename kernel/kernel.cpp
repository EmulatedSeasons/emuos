#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <arch/i386/tmtty.h>
#include <arch/i386/ps2_controller.h>
#include <dev/ps2kbd.h>

// linker symbols
unsigned int _kernel_begin;
unsigned int _kernel_end;


extern "C" void kernel_main(void) {
    // Initialize terminal
    //terminal_initialize();

    //initialize_ps2_controller();
    //keyboard_init();

    printf("Hello world!\n");
    printf("a%db\n", 1);
    printf("_begin: %x, _end: %x", &_kernel_begin, &_kernel_end);
}
