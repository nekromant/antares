#include <arch/antares.h>
#include "include/chassis.h"
//#include "include/manipulator.h"

static volatile int mot_a;
static volatile int mot_b;
static volatile int move;

static volatile int _enc0;
static volatile int _enc1;
static volatile int _deg;

static int global_dir0;
static int global_dir1;

static volatile int s=180;
static volatile int s1,s2;
static volatile int spd1=255, spd2=255;

static uint16_t distance_global;

static int g,j = 0;
#define _INF_ENABLED 1
#define _DBG_ENABLED 1
#define _ERR_ENABLED 1
#define _WNR_ENABLED 1

#include "include/debug.h"

#define pin_init_button(DDR,PORT,PIN) DDR&=~(1<<PIN); PORT|=(1<<PIN); 
#define pin_is_set(PORT, PIN) (PORT & (1<<PIN))

//static int gauss[5][9];
//unsigned char summ;

ANTARES_INIT_HIGH(chassis_init)
{
    DDRL|= 0x3 | (1 << 6) | (1 << 7);
    DDRL = DDRL| (1 << 3) | (1 << 4);
    PORTL = PORTL| (1 << 3) | (1 << 4);
    TCCR5A = TCCR5A| (1 << COM5A1) | (1 << COM5B1) | (1 << WGM50);
    TCCR5B = TCCR5B| (1 << CS51) | (1 << WGM52);
    pin_init_button(DDRD,PORTD,4);
    pin_init_button(DDRD,PORTD,5);
    pin_init_button(DDRD,PORTD,2);
    pin_init_button(DDRG,PORTG,3);
    pin_init_button(DDRG,PORTG,4);
    pin_init_button(DDRA,PORTA,0);
    pin_init_button(DDRA,PORTA,1);
    DBG("chassis: ready");
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
__inline void motor_set_dir(int num, int dir)
{
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

void motor_stop(int motor)
{
  if (!motor)
  PORTL&= ~(1<<0|1<<1|1<<6);
  else
  PORTL&= ~(1<<7|1<<3|1<<4);
}

void chassis_move(char dir0, char dir1, int pwm0, int pwm1, uint16_t enc0, uint16_t enc1)
{
  stop();
  _delay_ms(100);
  encoders_reset();
  chassis_move_simple(dir0, dir1, pwm0, pwm1);  
  _enc0 = enc0*7.3;
  _enc1 = enc1*7.3;
  
  while (1)
  {
    if (encoders_get(0) > _enc0 ) motor_stop(0);
    if (encoders_get(1) > _enc1 ) motor_stop(1);
    if ((encoders_get(0) > _enc0) || (encoders_get(1) > _enc1)) break;
  }
  stop();
  
}

void chassis_move_precise(int dir, uint8_t dist)
{
  stop();
  _delay_ms(100);
  encoders_reset();
  chassis_move_simple(dir, dir, spd1, spd2);  
  _enc0 = dist*7.3;
  distance_global = distance_global + dist;
  while((encoders_get(0) < _enc0) || (encoders_get(1) < _enc0))
  {
    if(encoders_get(0) < encoders_get(1))
    {
       if(s2<s-3){s2=s2+2;}else{s1=s1-2;}
    }
    if(encoders_get(0) > encoders_get(1))
    {
       if(s1<s-3){s1=s1+2;}else{s2=s2-2;}
    }
    if(encoders_get(0) == encoders_get(1))
    {
       s1=s; s2=s;
    }
    motor_set_speed(0, s1);
    motor_set_speed(1, s2); 
  }
  stop();
  _delay_ms(10);
}

__inline void distance_reset()
{
  distance_global = 0; 
}

__inline void get_distance()
{
  return distance_global;
}
/*
void chassis_move_precise(int dir0, uint8_t enc0)
{
  stop();
  _delay_ms(100);
  encoders_reset();
  chassis_move_simple(dir0, dir0, 255, 255);  

  _enc0 = enc0*7.3;
  while ((encoders_get(0) < _enc0) || (encoders_get(1) < _enc0))
  {
    if ((encoders_get(0) - encoders_get(1)) > 20) { chassis_move_simple(dir0, dir0, 255, 180); dump8(1); }
    if ((encoders_get(1) - encoders_get(0)) > 20) { chassis_move_simple(dir0, dir0, 180, 255); dump8(2); }
    chassis_move_simple(dir0, dir0, 255, 255);  
    //_delay_ms(100);
  }
  DBG("out");
  PORTJ=1<<7;
  //stop();
}
*/
void chassis_turn(int dir, int pwm, int deg)
{
  stop();
  _delay_ms(100);
  encoders_reset();
  if(dir == 0)
  {
    chassis_move_simple(1, 0, pwm, pwm);
  }
  else if(dir == 1)
  {
    chassis_move_simple(0, 1, pwm, pwm);
  }
  _deg = deg*1.57;
  while (1)
  {
    if (encoders_get(0) > _deg) motor_stop(0);
    if (encoders_get(1) > _deg) motor_stop(1);
    if ((encoders_get(0) > _deg) || (encoders_get(1) > _deg)) break;
  }
  stop();
}

void chassis_move_simple(char dir0, char dir1, int pwm0, int pwm1)
{
  stop();
  _delay_ms(100);
  motor_set_dir(0, dir0);
  motor_set_dir(1, dir1);
  motor_set_speed(0, pwm0);
  motor_set_speed(1, pwm1);
}

__inline void stop(){
   PORTL&= ~(1<<0|1<<1|1<<6|1<<7|1<<3|1<<4);
}
void reset_direction()
{
  chassis_move_simple(1, 1, 255, 255);
  mot_a = 1;
  mot_b = 1;
  while((mot_a == 1) || (mot_b == 1))
  {
     if (pin_is_set(PIND,4))
     {
        DBG("Pin 1 is SET");
     }
     else
     {
        motor_set_speed(0, 100);
	mot_a = 0;
     }
     if (pin_is_set(PIND,5))
     {
        DBG("Pin 2 is SET");
     } 
     else
     {
        motor_set_speed(1, 100);
	mot_b = 0;
     }
     if ((pin_is_set(PINA,0)) || (pin_is_set(PINA,1)))
     {
        DBG("Pin 1&2 is SET");
     }   
  }
  _delay_ms(100);
  stop();  
}
void chassis_find(char what, char side, int dir, uint8_t speed)
{
  move = 1;
  chassis_move_simple(dir, dir, speed, speed);
  if(side == left)
  {
    if(what == pawn)
    {
      while(move == 1)
      {
	if((read_barrier_rangefinders(l_b) == 0) && (read_barrier_rangefinders(l_t) == 1))
	{
	  stop();
	  move = 0;
	}
      }
    }
    else if((what == king) || (what == queen))
    {
      while(move == 1)
      {
        if((read_barrier_rangefinders(l_b) == 0) && (read_barrier_rangefinders(l_t) == 0))
        {
          stop();
	  move = 0;
        }
      }
    }
  }
  else if(side == right)
  {
    if(what == pawn)
    {
      while(move == 1)
      {
	if((read_barrier_rangefinders(r_b) == 0) && (read_barrier_rangefinders(r_t) == 1))
	{
	  stop();
	  move = 0;
        }
      }
    }
    else if((what == king) || (what == queen))
    {
      while(move == 1)
      {
        if((read_barrier_rangefinders(r_b) == 0) && (read_barrier_rangefinders(r_t) == 0))
        {
          stop();
	  move = 0;
        }
      }
    }
  }
  chassis_move(dir, dir, speed, speed, 10, 10);
}
int read_barrier_rangefinders(int num)
{
  j = 0;
  g = 0;
  if(num == l_t) 
  { 
    while(j < 30)
    {
      j++;
      if(pin_is_set(PING, 3)) 
      { 
	g++; 
      }  
      _delay_ms(5);
    }
  }
  if(num == l_b) 
  { 
    while(j < 30)
    {
      j++;
      if(pin_is_set(PING, 4)) 
      { 
        g++; 
      }  
      _delay_ms(5);
    }
  }
  if(num == r_b)
  { 
    while(j < 30)
    {
      j++;
      if(pin_is_set(PINA, 0)) 
      { 
        g++; 
      }  
      _delay_ms(5);
    }
  }
  if(num == r_t) 
  { 
    while(j < 30)
    {
      j++;
      if(pin_is_set(PINA, 1)) 
      { 
        g++; 
      }  
      _delay_ms(5);
    }
  }
  if(g == 0)
  {
    return 0; 
  }
  else
  {
    return 1;
  }
}

/*
int read_barrier_rangefinders(int num)
{
    if(num == l_t) { if(pin_is_set(PING, 3)) { return 1; } else { return 0; } }
    if(num == l_b) { if(pin_is_set(PING, 4)) { return 1; } else { return 0; } }
    if(num == r_b) { if(pin_is_set(PINA, 0)) { return 1; } else { return 0; } }
    if(num == r_t) { if(pin_is_set(PINA, 1)) { return 1; } else { return 0; } }
}
*/
/*
int read_barrier_rangefinders(int num)
{
    if(num == l_t) { if(pin_is_set(PING, 3)) { return Filter(1,1); } else { return Filter(1,0);}}
    if(num == l_b) { if(pin_is_set(PING, 4)) { return Filter(2,1); } else { return Filter(2,0);}}
    if(num == r_b) { if(pin_is_set(PINA, 0)) { return Filter(3,1); } else { return Filter(3,0);}}
    if(num == r_t) { if(pin_is_set(PINA, 1)) { return Filter(4,1); } else { return Filter(4,0);}}
}
int Filter(int Number, int currentValue)
{
    gauss[0][Number]=gauss[1][Number];
    gauss[1][Number]=gauss[2][Number];
    gauss[2][Number]=gauss[3][Number];
    gauss[3][Number]=gauss[4][Number];
    gauss[4][Number]=currentValue;
    summ=gauss[0][Number]+gauss[1][Number]+gauss[2][Number]+gauss[3][Number]+gauss[4][Number];
    if (summ>=3) return 1; else return 0;
}
*/