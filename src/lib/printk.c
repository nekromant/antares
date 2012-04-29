#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <lib/tasks.h>


void printk(const char *fmt, /*args*/ ...)
{
	printf("[%d	] ", tmgr_get_uptime());
	va_list ap;
	va_start(ap, fmt); 
	vprintf(fmt,ap);
	va_end(ap);
}