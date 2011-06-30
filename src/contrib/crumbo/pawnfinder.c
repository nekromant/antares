#include <arch/antares.h>
#include "include/start_cond.h"
#define _DBG_ENABLED 1
#define _INF_ENABLED 1
#include "include/debug.h"
#include "include/manipulator.h"
#include <lib/callchain/callchain.h>
#include <contrib/crumbo/systick.h>

static struct fcall_chain_t pfnd;
#define pin_init_button(DDR,PORT,PIN) DDR&=~(1<<PIN); PORT|=(1<<PIN); 
#define pin_init_sensor(DDR,PORT,PIN) DDR&=~(1<<PIN);  

#define pin_is_set(PORT, PIN) (PORT & (1<<PIN))
static volatile int flt[5];
static volatile char detected;
#define samplescount 6 

void p_reset()
{
  detected=0;
}

void proc_fnc()
{
  int i;
  if(!pin_is_set(PIND, 7)) flt[2]++; else flt[2]=0;
  if(!pin_is_set(PIND, 6)) flt[3]++; else flt[3]=0;
  if(!pin_is_set(PINA, 0)) flt[0]++; else flt[0]=0;
  if(!pin_is_set(PINA, 1)) flt[1]++; else flt[1]=0;
  for (i=0;i<4;i++)
  {
    
    if (flt[i]>samplescount) {
      detected|=(1<<i);
      flt[i]=0;
      //dump16(i);
    }
  }
}

__inline char have_something(int i)
{
  char r = (detected & (1<<i));
  detected &=~(1<<i);
  return r;
}

ANTARES_INIT_LOW(pawnfinder_init)
{
  pin_init_button(DDRD,PORTG,6);
  pin_init_button(DDRD,PORTG,7);
  pin_init_button(DDRA,PORTA,0);
  pin_init_button(DDRA,PORTA,1);
  pfnd.func=proc_fnc;
  systick_add(&pfnd);
  
}

//0 - right bot
//1 - right top
//2 - left bot
//3 - left top