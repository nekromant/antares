#ifndef LIB_PANIC_H
#define LIB_PANIC_H

#include <generic/macros.h>

#ifdef CONFIG_LIB_PANIC
void do_panic(const char* why);

void panic_user_hook(const char* why);


#if defined(TRACE_ASSERTS) || defined(CONFIG_LIB_PANIC_TRACE)

#define assert(expr) \
	((expr) ? (void) 0 : panic("assert '" QUOTE(expr)"' failed in " __FILE__ ":" \
				  EXPAND_AND_QUOTE(__LINE__) "\n"))

/* Linux kernel crowd - make yourself at home */

#define BUG_ON(expr) \
	((expr) ? panic("BUG ('" QUOTE(expr)"') in " __FILE__ ":" \
			EXPAND_AND_QUOTE(__LINE__) "\n") : (void)0 )

#else

#define assert(expr) \
	((expr) ? (void)0 : panic("assert failed\n"))

#define BUG_ON(expr) \
	((expr) ? panic("BUG detected\n") : (void)0 )

#endif /* defined(TRACE_ASSERTS) || defined(CONFIG_LIB_PANIC_TRACE) */

#ifdef CONFIG_ARCH_AVR
#include <avr/pgmspace.h>
#define panic(fmt, ...) do_panic(PSTR("REASON: " fmt))
#else
#define panic(fmt, ...) do_panic("REASON: " fmt)
#endif

#else 

#define assert(expr)
#define BUG_ON(expr)
#define panic(reason)

#endif /* CONFIG_LIB_PANIC */



#endif


