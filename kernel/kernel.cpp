#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <arch/i386/tmtty.h>
#include <arch/i386/ps2_controller.h>
#include <dev/ps2kbd.h>

// compiler and target check
#if defined(__linux__)
#error "you're not using a cross compiler"
#endif

#if !defined(__i386__)
#error "needs to be compiled using an ix86-elf cross compiler"
#endif

extern "C" void kernel_main(void) {
    // Initialize terminal
    //terminal_initialize();

    //initialize_ps2_controller();
    //keyboard_init();

    printf("Hello world!\n");
    printf("among\n");
    printf("balls\n");
    printf("a%db", 1);
}