#include <arch/antares.h>
//This is the startup file. Since it's the first one to compile/link, 

ANTARES_INIT_LOW(antares_low_init)
{
   //cli(); //Disable interrupts. Not needed really
}


//The first thing we do is: Turn on interrupts.
//All further code goes with interrupts enabled
ANTARES_INIT_HIGH(antares_high_init)
{
   //sei();
}

//This is a dummy app, so that we can call it from main, to start the loop
//over again. Since startup is the first one, we get it to run first

ANTARES_APP(antares_first_app)
{
  
}

ANTARES_FINISH(antares_exit)
{
    //Since we call main, avoid stack overflow
    //asm("pop r2;pop r2;");
    //_delay_ms(10);
    //comm_putc('a');
    //comm_sync();
    asm("jmp antares_first_app");
}

int main()
{
  //The main code must restart the app loop
  //NOTE: Move that to single tasking method
//  antares_first_app();
  //comm_putc('A');
  //_delay_ms(100);
return 0;
}
