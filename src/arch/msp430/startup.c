#include <arch/antares.h>
#include <intrinsics.h>

ANTARES_INIT_LOW(antares_low_init)
{
	/* Dummy */
}


/* The first thing we do is: Turn on interrupts. */
ANTARES_INIT_HIGH(antares_high_init)
{
        __eint();
}

/* This is a dummy app, so that we can call it from main, to start the loop
 * over again. Since startup is the first one, we get it to run first
 */

ANTARES_APP(antares_first_app)
{
  /* DUMMY */
}

ANTARES_FINISH(antares_exit)
{
    asm("jmp antares_first_app");
}

int main()
{
    /* Nothing in here for now */
    return 0;
}
