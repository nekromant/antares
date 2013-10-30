#include "lib/earlycon.h"
#include <stdio.h>

int putchar (int c) {   
	early_putc(c);
	return 0;
}

char getchar (void) { 
	return early_getc();
}


/* Dummy */
void early_console_glue_init()
{
	
}
