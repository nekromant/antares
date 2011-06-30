#include <arch/antares.h>
#include "include/pawnfinder.h"

#define _INF_ENABLED 0
#define _DBG_ENABLED 1
#define _ERR_ENABLED 0
#define _WNR_ENABLED 0

#include "include/debug.h"

static int mdir[2];

ANTARES_INIT_HIGH(motors_init)
{ 
    DBG("motors_ready");  
    DDRL= DDRL| (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6) | (1 << 7);
    PORTL = PORTL| (1 << 3) | (1 << 4);
    TCCR5A = TCCR5A| (1 << COM5A1) | (1 << COM5B1) | (1 << WGM50);
    TCCR5B = TCCR5B| (1 << CS51) | (1 << WGM52);
}

__inline void motor_set_speed(int num, uint8_t value)
{
    if(num == 1)
    {
      OCR5A=value;
    }
    if(num == 0)
    {
      OCR5B=value;
    }
}
static int mdir[2];

__inline int motor_get_dir(int num)
{
  return mdir[num];
}

__inline void motor_set_dir(int num, int dir)
{
  mdir[num]=dir;
  if(num)
  {
    if(dir)
    {
      PORTL|= 1<<0;
      PORTL&= ~(1<<1);
    }
    else
    {
      PORTL|= 1<<1;
      PORTL&= ~(1<<0);
    }
    }else
    {
      if(dir)
      {
	  PORTL|= 1<<6;
          PORTL&= ~(1<<7);
      }
      else
      {
	  PORTL|= 1<<7;
          PORTL&= ~(1<<6);
      }
    }
}

__inline void motor_stop(int motor)
{
  mdir[motor]=2;
  if (!motor)
  PORTL&= ~(1<<0|1<<1|1<<6);
  else
  PORTL&= ~(1<<7|1<<3|1<<4);
}

__inline void __stop(){
   PORTL&= ~(1<<0|1<<1|1<<6|1<<7|1<<3|1<<4);
}

__inline void stop()
{
  motor_stop(0);
  motor_stop(1);
}

#ifdef CONFIG_CONTRIB_CRUMBO_MOTORS_TEST

ANTARES_APP(motors_test)
{
  DBG("---motors test---");
  _delay_ms(3000);
  stop(); 
  DBG("1");
  motor_set_dir(0, 0);
  motor_set_dir(1, 0);
  motor_set_speed(0, 255);
  motor_set_speed(1, 255);  
  _delay_ms(3000);
  stop(); 
  DBG("2");
  motor_set_dir(0, 0);
  motor_set_dir(1, 0);
  motor_set_speed(0, 255);
  motor_set_speed(1, 255);  
  _delay_ms(3000);
  stop(); 
  DBG("3");
  motor_set_dir(0, 0);
  motor_set_dir(1, 0);
  motor_set_speed(0, 255);
  motor_set_speed(1, 255);  
  _delay_ms(3000);
  stop(); 
}

#endif

