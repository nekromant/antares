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


#ifdef CONFIG_CONTRIB_CRUMBO_SYSTICK_COUNTER
static volatile int cvalue;
static volatile int climit; 
static struct fcall_chain_t timeup_chain;
#endif

static void timeup()
{
   
}

__inline void systick_counter_set_limit(int _climit)
{
  #ifdef CONFIG_CONTRIB_CRUMBO_SYSTICK_TEST
  dump16(_climit);
  DBG("Setting up new timer limit");
  #endif
  climit=_climit;
  
}

__inline void systick_counter_reset()
{
  cvalue=0;
}

__inline uint16_t systick_counter_getticks()
{
  return cvalue;
}

static void systick_first()
{

  #ifdef CONFIG_CONTRIB_CRUMBO_SYSTICK_COUNTER
  cvalue++;
  
  #ifdef CONFIG_CONTRIB_CRUMBO_SYSTICK_TEST
  dump16(cvalue);
  dump16(climit);
  #endif
  
  #ifdef CONFIG_CONTRIB_CRUMBO_SYSTICK_CHANDLER
  if (cvalue==climit)
  {
    call_fuction_chain(&timeup_chain);
  }
  #endif
  #endif
  

  
}

__inline void systick_add(struct fcall_chain_t* f)
{
  call_function_add(&syschain,f);
}

__inline void systick_timeup_add(struct fcall_chain_t* f)
{
  call_function_add(&timeup_chain,f);
}

ANTARES_INIT_LOW(systick_init)
{
    syschain.func=systick_first;
    timeup_chain.func=timeup;
    TCCR4B=TCCR4B| (1 << CS42) | (1<<WGM42) | (1<<WGM43);
    TIMSK4=TIMSK4| (1 << ICIE4);
    ICR4=6250;
} // __inline void systick_init()


ISR(TIMER4_CAPT_vect) {
    call_fuction_chain(&syschain);
}