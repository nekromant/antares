#include <arch/antares.h>

/* 
 * We may be running from a bootloader and start with isrs enabled. 
 * Disable 'em all
 */

__antares_naked
__attribute__((__section__(".init5"))) void low_disable_isr(void) {		
	__asm("cli;");								
}


/* Turn on interrupts. */
__antares_naked
__attribute__((__section__(".init7"))) void high_enable_isr(void) {		
	__asm("sei;");								
}


/* 
 * This is a dummy app, so that we can call it from main, to start the loop
 * over again. Since startup is the first one, we get it to run first
 */
__antares_naked
__attribute__((__section__(".init8"))) void __antares_app_start(void) {

}

__antares_naked
__attribute__((__section__(".fini1"))) void _antares_exit(void)
{
#ifdef __AVR_HAVE_JMP_CALL__
	__asm("jmp __antares_app_start");
#else
	__asm("rjmp __antares_app_start");
#endif
}

#pragma GCC diagnostic push
/* Stop complaining about return type of main(). */
#pragma GCC diagnostic ignored "-Wmain"
void main()
{
	/* Nothing in here for now */
}
#pragma GCC diagnostic pop
