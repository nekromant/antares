#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <lib/tasks.h>
#include <lib/earlycon.h>


/* TODO: Use ARCH_HAS_STDIO instead */
#ifndef CONFIG_ARCH_8051
#define k_printf(fmt, ...) fprintf(p_stdout, fmt, #__VA_ARGS__) 
#define k_vprintf(fmt, ap) fprintf(p_stdout, fmt, ap)

#ifdef CONFIG_LIB_EARLYCON
static FILE* p_stdin = &g_early_stdin;
static FILE* p_stdout = &g_early_stdout;
#else
static FILE* p_stdin;
static FILE* p_stdout;
#endif

#else

#define k_printf(fmt, ...) printf(fmt, #__VA_ARGS__) 
#define k_vprintf(fmt, ap) printf(fmt, ap)

#endif


void printk(const char *fmt, /*args*/ ...)
{
#ifdef CONFIG_LIB_PRINTK_PREFIX
	k_printf(CONFIG_LIB_PRINTK_PREFIX_V);
#endif
	
#ifdef CONFIG_LIB_PRINTK_TIMESTAMP
	k_printf("[%d	] ", tmgr_get_uptime());
#endif
	va_list ap;
	va_start(ap, fmt); 
	k_vprintf(fmt, ap);
	va_end(ap);
}

