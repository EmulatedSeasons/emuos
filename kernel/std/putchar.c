#include <stdio.h>
 
//#include <arch/i386/tmtty.h>
#include <arch/x86_64/serial.h>
 
int putchar(int ic) {
	char c = (char) ic;
	serial_putchar(c);
	//terminal_write(&c, sizeof(c));
	return ic;
}