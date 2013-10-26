#ifndef EARLYCON_H
#define EARLYCON_H

struct early_console { 
	void (*init)(void);
	void (*txchar)(char c);
	int (*rxchar)(void);
};

void early_putc(char c);
int early_getc();

extern struct early_console g_early_console;

/* TODO: ARCH_HAS_STDIO */


#include <stdio.h>
extern FILE g_early_stdin;
extern FILE g_early_stdout;

#endif
