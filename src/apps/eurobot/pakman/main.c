#include <arch/antares.h>
#define _DBG_ENABLED 1
#define _INF_ENABLED 1
#include <contrib/crumbo/debug.h>
#include <contrib/crumbo/systick.h>
#include <contrib/crumbo/manipulator.h>
#include <contrib/crumbo/chassis.h>
#include <contrib/crumbo/adc.h>
#include <contrib/crumbo/odetect.h>
#include <contrib/crumbo/motors.h>
#include <contrib/crumbo/encoder.h>
#include <contrib/crumbo/strateg.h>

#include <lib/callchain/callchain.h>



static uint16_t distance_global;

//static char pl;
__inline void collision_avoidance(char state)
{
  comm_putc(state);
  comm_putc(PORTL);
  switch(state)
  {
    case 1:
    __stop();
    break;
    case 0:
    if (motor_get_dir(0)!=2) motor_set_dir(0, motor_get_dir(0));
    if (motor_get_dir(1)!=2) motor_set_dir(1, motor_get_dir(1));
    break;
  }
  //state=1 - have something
  //state=0 - all clear
}



ANTARES_INIT_LOW(pakman_init)
 {
 //code here
 }


ANTARES_APP(pakman_mainapp)
 {
    DDRJ|=1<<7;
    PORTJ&=~(1<<7);
    DBG("Pakman ready, using Antares v." CONFIG_VERSION_STRING " git commit: " CONFIG_VERSION_GIT );
    #ifdef CONFIG_CONTRIB_CRUMBO_STRATEGY_GUIDE
    execute_strategy();
    #endif
}

void count_distance()
{
  distance_global = (encoders_get(0) + encoders_get(1)) / 2 / 7.3;
  //dump16(encoders_get(0));
  //dump16(encoders_get(1));
  //dump16(distance_global);
  _delay_ms(3000); 
}
inline void reset_distance()
{
  distance_global = 0;
}

