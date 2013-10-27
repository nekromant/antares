#ifndef EARLYCON_H
#define EARLYCON_H

struct early_console { 
	void (*init)(void);
	void (*txchar)(char c);
	int (*rxchar)(void);
	int (*havechar)(void);
};

void early_console_checkinit();
void early_putc(char c);
int early_getc();
int early_avail();

extern struct early_console g_early_console;

/* TODO: Use ARCH_HAS_STDIO into account */

#if !defined(CONFIG_ARCH_8051) && !defined(CONFIG_ARCH_MSP430)
#include <stdio.h>
extern FILE *g_early_stdin;
extern FILE *g_early_stdout;
#endif

#endif
