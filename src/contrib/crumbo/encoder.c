// TODO: Legal stuff goes here
// This file is automagically generated, dude
// I wouldn't touch that if I were you, since all the stuff you'll fix
// Will eventually go down the closet
#include <arch/antares.h>

#define _INF_ENABLED 1
#define _DBG_ENABLED 1
#define _ERR_ENABLED 1
#define _WNR_ENABLED 1
#include "include/debug.h"
// generating encoder library for 1 interrupts
// generating encoder library for 2 interrupts
static uint16_t counters[2];
ANTARES_INIT_LOW(encoders_init)
{
    EICRB = EICRB | (1<<ISC60)| (1<<ISC70);
    EIMSK = EIMSK | (1<<6)| (1<<7);
} // encoders_init()
//////////
ISR(INT6_vect) {
    counters[0]++;
}
//////////
ISR(INT7_vect) {
    counters[1]++;
}
void encoders_reset()
{
    counters[0]=0;
    counters[1]=0;
} // encoders_reset()
__inline uint16_t encoders_get(int num)
{
    return counters[num];
} // uint16_t encoders_get(int num)


#ifdef CONFIG_CONTRIB_CRUMBO_ENCODERS_TEST

ANTARES_APP(encoders_test)
{
  DBG("---encoders test---");
  dump16(encoders_get(0));
  dump16(encoders_get(1));
  _delay_ms(100);
}

#endif

