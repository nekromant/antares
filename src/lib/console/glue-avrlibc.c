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

FILE *g_early_stdin;
FILE *g_early_stdout;

static FILE early_stdin = FDEV_SETUP_STREAM(NULL, avr_early_getc,
                                             _FDEV_SETUP_READ);
static FILE early_stdout = FDEV_SETUP_STREAM(avr_early_putc, NULL,
                                             _FDEV_SETUP_WRITE);

void early_console_glue_init() {
	g_early_stdin = &early_stdin;
	g_early_stdout = &early_stdout;
}

