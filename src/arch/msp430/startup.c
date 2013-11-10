#include <arch/antares.h>

/* The first thing we do is: Turn on interrupts. */
__attribute__((naked))							
__attribute__((__section__(".init7"))) void high_enable_isr(void) {		
	__eint();;								
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
    asm("jmp __antares_app_start");
}

int main()
{
    /* Nothing in here for now */
    return 0;
}
