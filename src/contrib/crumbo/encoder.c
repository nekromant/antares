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
    //DDRD&=(1<<0)|(1<<1);
    //PORTD|=(1<<0)|(1<<1);
    
//     EICRB = EICRB | (1<<ISC60)| (1<<ISC70);
     EICRA = EICRA | (1<<ISC20)| (1<<ISC30);
//      EIMSK = EIMSK | (1<<6)| (1<<7);
     EIMSK = EIMSK | (1<<2) | (1<<3);
//     
} // encoders_init()
//////////
ISR(INT2_vect) {
    counters[0]++;
}
//////////
ISR(INT3_vect) {
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
#warning ertyui
ANTARES_APP(encoders_test)
{
 // comm_puts("---encoders test---");
  dump16(encoders_get(0));
  dump16(encoders_get(1));
  _delay_ms(300);
}

#endif

