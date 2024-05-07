#include <stdio.h>
 
//#include <arch/i386/tmtty.h>
 
int putchar(int ic) {
	char c = (char) ic;
	//terminal_write(&c, sizeof(c));
	return ic;
}