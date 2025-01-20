#include <arch/x86_64/io.h>
#include <string.h>

#define PORT 0x3f8

int is_transmit_empty() {
    return inb(PORT + 5) & 0x20;
}

extern "C" void serial_putchar(char c) {
    while (is_transmit_empty() == 0);
    outb(PORT, c);
}

void serial_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        serial_putchar(data[i]);
    }
}

void serial_writestring(const char* data) {
    serial_write(data, strlen(data));
}