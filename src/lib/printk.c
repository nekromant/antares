#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <lib/tasks.h>


void printk(const char *fmt, /*args*/ ...)
{
#ifdef CONFIG_LIB_PRINTK_PREFIX
	printf(CONFIG_LIB_PRINTK_PREFIX_V);
#endif
	
#ifdef CONFIG_LIB_PRINTK_TIMESTAMP
	printf("[%d	] ", tmgr_get_uptime());
#endif
	va_list ap;
	va_start(ap, fmt); 
	vprintf(fmt,ap);
	va_end(ap);
}



