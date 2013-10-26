#include "lib/earlycon.h"
#include <stdio.h>
#include <arch/antares.h>

static int avr_early_putc(char c, FILE *stream)
{
	early_putc(c);
	return 0;
}

static int avr_early_getc(FILE *stream)
{
	return early_getc();
}


FILE g_early_stdin = FDEV_SETUP_STREAM(NULL, avr_early_getc,
                                             _FDEV_SETUP_READ);
FILE g_early_stdout = FDEV_SETUP_STREAM(avr_early_putc, NULL,
                                             _FDEV_SETUP_WRITE);
