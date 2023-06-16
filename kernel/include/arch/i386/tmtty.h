#ifndef TEXTMODE_TTY_H
#define TEXTMODE_TTY_H

#include <stddef.h>

/* Text mode tty */

#ifdef __cplusplus
extern "C"
{
#endif

void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);

#ifdef __cplusplus
}
#endif

#endif