#include <arch/antares.h>
#include "include/start_cond.h"
#define _DBG_ENABLED 1
#define _INF_ENABLED 1
#include "include/debug.h"
#include "include/manipulator.h"
#include <lib/callchain/callchain.h>
#include <contrib/crumbo/systick.h>

static volatile char st;
static struct fcall_chain_t timeupc;

static void timeup_handler()
{
  switch(st)
  {
    case 0:
      systick_counter_set_limit(5);
      systick_counter_reset();
      OCR1A = map(170, 0, 180, 0, 255);
      OCR1B = map(84, 0, 180, 0, 255);
      //85 code here
      break;
    case 1:
      stop();
      DBG("Time's up!");
      break;
  }
  st++;
}


ANTARES_INIT_HIGH(start_condition)
{
  STARTDDR&=~(STARTNUM);
  STARTPORT|=(STARTNUM);
  INF("Please, insert coin. ");
  while ((STARTPIN & (STARTNUM)));;
  INF("Thanks, dude, let me position myself...");
  _delay_ms(500);
  reset_direction();
  chassis_move_precise(0, 4);
  manipulator(move, stdby);
  manipulator(grip, grip);
  PORTJ|=1<<7;
  _delay_ms(500);
  INF("Waiting for start condition");
  while (!(STARTPIN & (STARTNUM)));;
  INF("Got it, gotta run!");
  timeupc.func=timeup_handler;
  systick_timeup_add(&timeupc);
  systick_counter_initsimple(85);
  st=0;
}
