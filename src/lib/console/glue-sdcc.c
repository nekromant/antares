#include "lib/earlycon.h"
#include <stdio.h>

void putchar (char c) {   
	early_putc(c);
}

char getchar (void) { 
	return early_getc();
}



