#include <arch/antares.h>
#include <lib/panic.h>
#include <lib/printk.h>
#include <lib/earlycon.h>

void do_panic(const char* why)
{
#ifdef CONFIG_LIB_PANIC_PRINTOUT
	printk("\nFATAL: SYSTEM PANIC \n");
	panic_printk(why);
#endif

#ifdef CONFIG_LIB_PANIC_HOOK
	panic_user_hook(why);
#endif
	while(1);;
}
