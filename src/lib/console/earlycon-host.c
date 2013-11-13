#include <termios.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <lib/earlycon.h>
#include <lib/printk.h>

static void stdio_putchar(char c)
{
	putchar(c);
	fflush(stdout);
}

static int stdio_getchar()
{
	int c; 
	do { 
		c = getchar();
	} while (c == EOF);
	return c;
}

static void stdio_init() 
{
	fprintf(stderr, "earlycon: native init..\n");
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

static int stdio_avail()
{
	int c = getchar();
	if (c != EOF)
		ungetc(c, stdin);
	return (c != EOF);
}


struct early_console g_early_console = {
	.rxchar = stdio_getchar,
	.txchar = stdio_putchar,
	.init = stdio_init,		
	.havechar = stdio_avail,
};
