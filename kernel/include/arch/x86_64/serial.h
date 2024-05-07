#ifndef SERIAL_H
#define SERIAL_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

void serial_putchar(char c);
void serial_write(const char* data, size_t size);
void serial_writestring(const char* data);

#ifdef __cplusplus
}
#endif

#endif