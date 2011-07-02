#include <arch/antares.h>

#define _INF_ENABLED 1
#define _DBG_ENABLED 1
#define _ERR_ENABLED 1
#define _WNR_ENABLED 1
#include "include/debug.h"

#define pin_init_button(DDR,PORT,PIN) DDR&=~(1<<PIN); PORT|=(1<<PIN);
#define pin_is_set(PORT, PIN) (PORT & (1<<PIN))


#include "include/manipulator.h"
#include "include/motors.h"


ANTARES_INIT_LOW(manipulator_init)
{
    EICRA|= (1<<ISC30);
    DDRB=DDRB| (1 << 4) | (1 << 5)| (1 << 6);
    PORTB=PORTB| (1 << 4) | (1 << 5)| (1 << 6);
    DDRH=DDRH| (1 << 6);
    DDRC|=(1<<0)|(1<<1);
    PORTC=PORTC| (1 << 0)|(1 << 1);
    PORTC&=~(1 << 0);
    PORTC&=~(1 << 1);
    PORTH=PORTH| (1 << 6);
    pin_init_button(DDRD,PORTD,1);
    TCNT1 = 0xB800;
    ICR1 = 0x0900;
    TCCR1A = _BV(COM1A1) | !_BV(COM1A0)           //   Both PWM outputs set at TOP,
             | _BV(COM1B1) | !_BV(COM1B0)            //   clear on compare match
             | !_BV(FOC1A) | !_BV(FOC1B)             //   PWM mode, can't force output
             | !_BV(WGM11) | !_BV(WGM10);            //   Fast PWM, TOP = ICR1
    TCCR1B = !_BV(ICNC1) | !_BV(ICES1)            //   Disable input capture noise canceler,
             //   edge select to negative.
             | _BV(WGM13) | !_BV(WGM12)                    //   Fast PWM, TOP = ICR1
             | !_BV(CS12) | _BV(CS11) | _BV(CS10);         //   clk(i/o) / 64
}


long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void manipulator(char task, char type)
{
    switch(task)
    {
      case move:
        if(type == stdby)
        {
            OCR1A = map(60, 0, 180, 0, 255);
            _delay_ms(1000);
        }
        else if(type == mid)
        {
            OCR1A = map(113, 0, 180, 0, 255);
            _delay_ms(1000);
        }
        else if(type == mid2)
        {
            OCR1A = map(85, 0, 180, 0, 255);
            _delay_ms(1000);
        }
        else if(type == bot)
        {
            OCR1A = map(170, 0, 180, 0, 255);
            _delay_ms(1000);
        }
    break;
      case grip:
        if(type == grip)
        {
            OCR1B = map(143, 0, 180, 0, 255);
            _delay_ms(500);
            OCR1B = map(120, 0, 180, 0, 255);
            _delay_ms(500);
            OCR1B = map(150, 0, 180, 0, 255);
            _delay_ms(500);
        }
        else if(type == release)
        {
            OCR1B = map(84, 0, 180, 0, 255);
            _delay_ms(1000);
        }
        else if(type == drop)
        {
            OCR1B = map(84, 0, 180, 0, 255);
            OCR1A = map(170, 0, 180, 0, 255);
            _delay_ms(1000);
        }

    break;
      case get_a_pawn:
        motor_set_speed(0, 255);
        motor_set_speed(1, 255);
	motor_set_dir(0, 0);
	motor_set_dir(1, 0);	
	while(pin_is_set(PIND, 1));
        stop();
        break;
      case stand:
        if(type == open)
        {
            //PORTC&=~(1<<1);
            PORTC|=(1<<0);
            _delay_ms(3000);
            PORTC&=~(1<<0);
        }
        else if(type == close)
        {
            //PORTC&=~(1<<0);
            PORTC|=(1<<1);
            _delay_ms(3000);
            PORTC&=~(1<<1);
        }
    break;
    }
}