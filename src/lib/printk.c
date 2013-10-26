#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <lib/tasks.h>
#include <lib/earlycon.h>


#ifdef CONFIG_LIB_EARLYCON
static FILE* p_stdin = &g_early_stdin;
static FILE* p_stdout = &g_early_stdout;
#else
static FILE* p_stdin;
static FILE* p_stdout;
#endif


void printk(const char *fmt, /*args*/ ...)
{
#ifdef CONFIG_LIB_PRINTK_PREFIX
	fprintf(p_stdout, CONFIG_LIB_PRINTK_PREFIX_V);
#endif
	
#ifdef CONFIG_LIB_PRINTK_TIMESTAMP
	fprintf(p_stdout, "[%d	] ", tmgr_get_uptime());
#endif
	va_list ap;
	va_start(ap, fmt); 
	vfprintf(p_stdout, fmt, ap);
	va_end(ap);
}

