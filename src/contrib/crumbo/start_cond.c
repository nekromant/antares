#include <arch/antares.h>
#include "include/start_cond.h"
#define _DBG_ENABLED 1
#include "include/debug.h"
#include "include/manipulator.h"


ANTARES_INIT_HIGH(start_condition)
{
  STARTDDR&=~(STARTNUM);
  STARTPORT|=(STARTNUM);
  DBG("Please, insert coin. ");
  while ((STARTPIN & (STARTNUM)));;
  DBG("Thanks, dude");
  _delay_ms(500);
  reset_direction();
  chassis_move_precise(0, 4);
  manipulator(move, stdby);
  manipulator(grip, grip);
  PORTJ|=1<<7;
  _delay_ms(500);
  DBG("Waiting for start condition");
  while (!(STARTPIN & (STARTNUM)));;
  DBG("Got it, gotta run!");
}
