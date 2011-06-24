// TODO: Legal stuff goes here
// This file is automagically generated, dude
// I wouldn't touch that if I were you, since all the stuff you'll fix
// Will eventually go down the closet
#include <arch/antares.h>
#include "include/systick.h"
#define _DBG_ENABLED 1
#include "include/debug.h"
#include <lib/callchain/callchain.h>

static struct fcall_chain_t syschain;
static struct fcall_chain_t syschain2;
static struct fcall_chain_t syschain3;

static void systick_first()
{
  //comm_putc('A');
  
}



void systick_add(struct fcall_chain_t* f)
{
  call_function_add(&syschain,f);
}

ANTARES_INIT_LOW(systick_init)
{
    syschain.func=systick_first;
    TCCR1B=TCCR1B| (1 << CS11);
    TIMSK1=TIMSK1| (1 << TOIE0);
    //DBG("Systick timer got enabled");
} // __inline void systick_init()


ISR(TIMER1_OVF_vect) {
    call_fuction_chain(&syschain);
}
