#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <lib/earlycon.h>

#ifdef CONFIG_LIB_PRINTK_TIMESTAMP
#include <lib/tmgr.h>
#endif

/* TODO: Use ARCH_HAS_STDIO instead */
#if !defined(CONFIG_ARCH_8051) && !defined(CONFIG_ARCH_MSP430)
#define k_printf(fmt, ...) fprintf(*p_stdout, fmt, ##__VA_ARGS__) 
#define k_vprintf(fmt, ap) vfprintf(*p_stdout, fmt, ap)

#ifdef CONFIG_LIB_EARLYCON
static FILE **p_stdin = &g_early_stdin;
static FILE **p_stdout = &g_early_stdout;
#define CHECK_STDOUT					\
	{						\
		if (*p_stdout == NULL) {		\
			early_console_checkinit();	\
		}					\
	}						\


#else /* CONFIG_LIB_EARLYCON */

#define CHECK_STDOUT					\
	{						\
		if (*p_stdout == NULL) {		\
			return;				\
		}					\
	}						\


static FILE **p_stdin;
static FILE **p_stdout;
#endif /* CONFIG_LIB_EARLYCON */

		
#else /* No proper stdio */

#define k_printf(fmt, ...) printf(fmt, ##__VA_ARGS__) 
#define k_vprintf(fmt, ap) vprintf(fmt, ap)

#define CHECK_STDOUT


#endif


/* AVR Needs extra love for PROGMEM/EEMEM */

#ifdef CONFIG_ARCH_AVR
#define k_printf_P(fmt, ...) fprintf_P(*p_stdout, fmt, ##__VA_ARGS__) 
#define k_vprintf_P(fmt, ap) vfprintf_P(*p_stdout, fmt, ap)
#define k_printf_E(fmt, ...) fprintf_E(*p_stdout, fmt, ##__VA_ARGS__) 
#define k_vprintf_E(fmt, ap) vfprintf_E(*p_stdout, fmt, ap)
#endif


static void printk_prefix()
{
	CHECK_STDOUT;

#ifdef CONFIG_LIB_PRINTK_PREFIX
#ifdef CONFIG_ARCH_AVR
	k_printf_P(PSTR(CONFIG_LIB_PRINTK_PREFIX_V));
#else
	k_printf(CONFIG_LIB_PRINTK_PREFIX_V);
#endif
#endif

	
#ifdef CONFIG_LIB_PRINTK_TIMESTAMP
#ifdef CONFIG_ARCH_AVR
	k_printf_P("[%u	] ", (unsigned int) tmgr_get_uptime());
#else
	k_printf("[%u	] ", (unsigned int) tmgr_get_uptime());
#endif
#endif
}

void printk_R(const char *fmt, /*args*/ ...) 
{
	va_list ap;
	va_start(ap, fmt); 
	printk_prefix();
	k_vprintf(fmt, ap);
	va_end(ap);
}

#ifdef CONFIG_ARCH_AVR
void printk_P(const char *fmt, /*args*/ ...)
{
	va_list ap;
	printk_prefix();
	va_start(ap, fmt); 
	k_vprintf_P(fmt, ap);
	va_end(ap);
}
#endif
