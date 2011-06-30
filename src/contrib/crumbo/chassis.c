#include <arch/antares.h>
#include "include/chassis.h"
#include "include/pawnfinder.h"
#include "include/feedback.h"
#include "include/motors.h"
#include "include/encoder.h"
#include "include/odetect.h"

//#include "include/manipulator.h"

static volatile int mot_a;
static volatile int mot_b;
static volatile int move;

static volatile int _enc0;
static volatile int _enc1;
static volatile int _deg;

static volatile int s=180;
static volatile int s1,s2;

#define _INF_ENABLED 0
#define _DBG_ENABLED 1
#define _ERR_ENABLED 0
#define _WNR_ENABLED 0

#include "include/debug.h"

#define pin_init_button(DDR,PORT,PIN) DDR&=~(1<<PIN); PORT|=(1<<PIN); 
#define pin_is_set(PORT, PIN) (PORT & (1<<PIN))

ANTARES_LINK_RULE("module chassis after init init2 init3")

ANTARES_INIT_HIGH(chassis_init)
{
    pin_init_button(DDRD,PORTD,4); //back 
    pin_init_button(DDRD,PORTD,5); //limit buttons
    pin_init_button(DDRD,PORTD,2); //forward button
    DBG("chassis: ready");
}

void chassis_move_precise_d(int dir, uint8_t perfect_speed, int b)
{
  _delay_ms(100);
  feedback_set_reference(perfect_speed);
  encoders_reset();
  feedback_process();
  motor_set_dir(0, dir);
  motor_set_dir(1, dir);
  //while(!(have_something(b)))
  //{
//    feedback_process();
//  }
  stop();
  chassis_move_precise(dir,perfect_speed,10);
}


void chassis_move_precise(int dir, uint8_t perfect_speed, uint8_t dist)
{
  _delay_ms(100);
  feedback_set_reference(perfect_speed);
  encoders_reset();
  feedback_process();
  motor_set_dir(0, dir);
  motor_set_dir(1, dir);
  _enc0 = dist*7.3;
  while((encoders_get(0) < _enc0) || (encoders_get(1) < _enc0))
  {
    feedback_process();
  }
  stop();
}

float coeff=1.43;
#ifdef CONFIG_CONTRIB_CRUMBO_TURN_CALIBRATE
ANTARES_APP(turn_calibrate)
{
  DBG("Calibrating turn.");
  char a=0;
  //for (coeff=1.44;coeff<1.50;coeff+=0.01)
  //{
  //dump8(a++);
  chassis_turn(1, 180, 360);
  chassis_turn(0, 180, 360);
  //DBG("Next");
  _delay_ms(10000);
  //}
}
#endif

/*
void chassis_turn(int dir, uint8_t perfect_speed, int deg)
{
  DBG("turn");
  _delay_ms(100);
  
  feedback_set_reference(perfect_speed);
  
  motor_set_speed(0, perfect_speed);
  motor_set_speed(1, perfect_speed);
  motor_set_dir(0, !dir);
  motor_set_dir(1, dir);
//   _delay_ms(500);
  _deg = deg * coeff;
  encoders_reset();
  while((encoders_get(0) > deg) && (encoders_get(1) > deg))
  {
    //feedback_process();
  }
  stop();
}
*/
void chassis_turn(int dir, int pwm, int deg)
{
  stop();
  _delay_ms(100);
  encoders_reset();
  //chassis_move_simple(!dir, dir, pwm, pwm);  
  motor_set_dir(0, !dir);
  motor_set_dir(1, dir);
  motor_set_speed(0, pwm);
  motor_set_speed(1, pwm);
  
  _deg = (int)(((float) deg) * coeff);
  while (1)
  {
    if(!odct_get_collision_state())
    {
      if ((encoders_get(0) > _deg) || (encoders_get(1) > _deg)) {stop(); break;};
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
    }
}


void reset_direction()
{
  char fix_disable=0;
  _delay_ms(100);
  encoders_reset();
  motor_set_speed(0, 255);
  motor_set_speed(1, 255);
  motor_set_dir(0, 1);
  motor_set_dir(1, 1);
  //mot_a = 1;
  //mot_b = 1;
  
  while((pin_is_set(PIND,4) && pin_is_set(PIND,5)))
  {
     if (!pin_is_set(PIND,4))
     {
        motor_set_speed(0, 100);
	//mot_a = 0;
	fix_disable=1;
     }
     if (!pin_is_set(PIND,5))
     {
        motor_set_speed(1, 100);
	//mot_b = 0;
	fix_disable=1;
     }
//      if ((pin_is_set(PINA,0)) || (pin_is_set(PINA,1)))
//      {
// //        DBG("Pin 1&2 is SET");
//      }
     if(!fix_disable) feedback_process();
  }
  _delay_ms(100);
  stop();  
}
/*
void chassis_find(char what, char side, int dir, uint8_t speed)
{
  encoders_reset();
  move = 1;
  chassis_move_simple(dir, dir, speed, speed);
  if(side == left)
  {
    if(what == pawn)
    {
      while(move == 1)
      {
	if((have_something(l_b) == 0) && (have_something(l_t) == 1))
	{
	  stop();
	  move = 0;
	}
    if(!odct_get_collision_state())
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
    }
    }
    else if((what == king) || (what == queen))
    {
      while(move == 1)
      {
        if((have_something(l_b) == 0) && (have_something(l_t) == 0))
        {
          stop();
	  move = 0;
        }
    if(!odct_get_collision_state())
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
      }
    }
  }
  else if(side == right)
  {
    if(what == pawn)
    {
      while(move == 1)
      {
	if((have_something(r_b) == 0) && (have_something(r_t) == 1))
	{
	  stop();
	  move = 0;
        }
     if(!odct_get_collision_state())
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
      }
    }
    else if((what == king) || (what == queen))
    {
      while(move == 1)
      {
        if((have_something(r_b) == 0) && (have_something(r_t) == 0))
        {
          stop();
	  move = 0;
        }
    if(!odct_get_collision_state())
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
      }
    }
  }
  //encoderik0 = encoders_get(0);
  //encoderik1 = encoders_get(1);
  //chassis_move(dir, dir, speed, speed, 10, 10);
  //distance_global = (((encoderik0 + encoderik1) / 2) / 7.3) + 10;
}
__inline void distance_reset()
{
  distance_global = 0; 
}

uint8_t get_distance()
{
  return distance_global;
}

//PING 3 4 - lt, lb
//A0 A1 - rb, rt



int read_barrier_rangefinders(int num)
{
  j = 0;
  g = 0;
  if(num == l_t) 
  { 
    while(j < 50)
    {
      j++;
      if(pin_is_set(PING, 3)) 
      { 
	g++; 
      }  
      _delay_ms(20);
    }
  }
  if(num == l_b) 
  { 
    while(j < 20)
    {
      j++;
      if(pin_is_set(PING, 4)) 
      { 
        g++; 
      }  
      _delay_ms(2);
    }
  }
  if(num == r_b)
  { 
    while(j < 20)
    {
      j++;
      if(pin_is_set(PINA, 0)) 
      { 
        g++; 
      }  
      _delay_ms(2);
    }
  }
  if(num == r_t) 
  { 
    while(j < 20)
    {
      j++;
      if(pin_is_set(PINA, 1)) 
      { 
        g++; 
      }  
      _delay_ms(2);
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


int read_barrier_rangefinders(int num)
{
    if(num == l_t) { if(pin_is_set(PING, 3)) { return 1; } else { return 0; } }
    if(num == l_b) { if(pin_is_set(PING, 4)) { return 1; } else { return 0; } }
    if(num == r_b) { if(pin_is_set(PINA, 0)) { return 1; } else { return 0; } }
    if(num == r_t) { if(pin_is_set(PINA, 1)) { return 1; } else { return 0; } }
}
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
