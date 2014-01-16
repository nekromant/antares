#include <stdint.h>
#include <lib/earlycon.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <arch/antares.h>

FILE* g_early_stdin;
FILE* g_early_stdout;

static ssize_t cookie_read_function(void *_cookie, char *_buf, size_t n)
{
	size_t i = 0;
	while (i < n) { 
		*_buf++ = (char) early_getc();
	} ;
	return n;
}

static ssize_t cookie_write_function(void *_cookie, const char *_buf, size_t n)
{
	size_t i = 0;
	while (i < n) 
		early_putc(_buf[i++]);
	return n;
}


static int cookie_close_function(void *cookie)
{
	return 0;
}

void early_console_glue_init() {
	cookie_io_functions_t ops_read = {
		.read = cookie_read_function,
		.write = NULL, 
		.seek = NULL,
		.close = cookie_close_function
	};
	cookie_io_functions_t ops_write = {
		.read = NULL,
		.write = cookie_write_function,
		.seek = NULL,
		.close = cookie_close_function	
	};	
	g_early_stdin  = fopencookie(NULL, "r", ops_read);
	g_early_stdout = fopencookie(NULL, "w", ops_write);
	setvbuf(g_early_stdout, NULL, _IONBF, 0);
        setvbuf(g_early_stdin,  NULL, _IONBF, 0);
}
