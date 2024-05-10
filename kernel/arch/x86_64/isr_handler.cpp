#include <stdint.h>
#include <stdio.h>

extern "C" void exception_handler(int e) {
    printf("Exception: %x", e);
}