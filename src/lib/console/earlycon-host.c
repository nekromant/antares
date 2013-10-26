#include <termios.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <lib/earlycon.h>
#include <lib/printk.h>

static void stdio_putchar(char c)
{
	putchar(c);
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
	struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");	
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
