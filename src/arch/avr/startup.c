#include <arch/antares.h>
//This is the startup file. Since it's the first one to compile/link, 

__attribute__((naked))							
__attribute__((__section__(".init5"))) void low_disable_isr(void) {		
	asm("cli;");								
}


//The first thing we do is: Turn on interrupts.
//All further code goes with interrupts enabled
/* The first thing we do is: Turn on interrupts. */
__attribute__((naked))							
__attribute__((__section__(".init7"))) void high_enable_isr(void) {		
	asm("sei;");								
}

//This is a dummy app, so that we can call it from main, to start the loop
//over again. Since startup is the first one, we get it to run first

ANTARES_APP(antares_first_app)
{
 //PORTG|=1<<3; 
}

ANTARES_FINISH(antares_exit)
{
    //Since we call main, avoid stack overflow
    //asm("pop r2;pop r2;");
    //_delay_ms(10);
    //comm_putc('a');
    //comm_sync();
    asm("rjmp .init8");
    //asm("jmp .init8");
}

int main()
{
	/* Nothing in here for now */
	return 0;
}
