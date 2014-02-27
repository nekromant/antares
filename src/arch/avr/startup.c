#include <arch/antares.h>

/* 
 * We may be running from a bootloader and start with isrs enabled. 
 * Disable 'em all
 */

__attribute__((naked))							
__attribute__((__section__(".init5"))) void low_disable_isr(void) {		
	__asm("cli;");								
}


/* Turn on interrupts. */
__attribute__((naked))							
__attribute__((__section__(".init7"))) void high_enable_isr(void) {		
	__asm("sei;");								
}


/* 
 * This is a dummy app, so that we can call it from main, to start the loop
 * over again. Since startup is the first one, we get it to run first
 */
__attribute__((naked))
__attribute__((__section__(".init8"))) void __antares_app_start(void) {

}

ANTARES_FINISH(antares_exit)
{
#ifdef __AVR_HAVE_JMP_CALL__
	__asm("jmp __antares_app_start");
#else
	__asm("rjmp __antares_app_start");
#endif
}

int main()
{
	/* Nothing in here for now */
	return 0;
}
