#include <arch/antares.h>
#include "include/pawnfinder.h"
#include "include/odetect.h"
#include "include/encoder.h"
#include "include/motors.h"
#include "include/systick.h"


//#define _INF_ENABLED 0
//#define _DBG_ENABLED 1
//#define _ERR_ENABLED 0
//#define _WNR_ENABLED 0

#include "include/debug.h"

static uint8_t ref=180;
static uint8_t spd1,spd2;


void feedback_reset()
{
 spd1=0;
 spd2=0; 
}
void feedback_process()
{
  if(!odct_get_collision_state())
  {
    if(encoders_get(0) < encoders_get(1))
    {
      if(spd2 < (ref-3)) { spd2 = spd2+2; } else { spd1 = spd1-2;}
    }
    if(encoders_get(0) > encoders_get(1))
    {
      if(spd1 < (ref-3)) { spd1 = spd1+2; } else { spd2 = spd2-2;}
    }
    if(encoders_get(0) == encoders_get(1))
    {
      spd1=ref; spd2=ref;
    }
    
    motor_set_speed(0, spd1);
    motor_set_speed(1, spd2); 
    /*
    DBG("feedbacking'");
    dump8(spd1);		
    dump8(spd2);		
    dump16(encoders_get(0));		
    dump16(encoders_get(1));		
    */
  }
}

ANTARES_INIT_HIGH(feedback_init)
{ 
  DBG("feedback is ready to work");
  ref = 180;
}

__inline void feedback_set_reference(uint8_t _ref)
{
  if(_ref > 230) ref = 230;
  ref = _ref; 
}

#ifdef CONFIG_CONTRIB_CRUMBO_FEEDBACK_TEST

ANTARES_APP(feedback_test)
{
  DBG("feedback processor test");
  motor_set_dir(0, 0);
  motor_set_dir(1, 0);
  while(1)
  {
    feedback_process();
  }
  stop();
}

#endif

