#include <arch/antares.h>
#include "include/servotool.h"

//Init the pins
//We do not use macros here, since it will be less code for uC this way
ANTARES_INIT_LOW(servotool_init_pins)
{
 
 DDRB=0xFF; 
 DDRD=1<<6|1<<5|1<<4|1|1<<1; //PD2 & PD3 are the USB
 //Power initially off
 //DDRB |= (1<<PB4)|(1<<PB3); 
 PORTB=0xFF; //Mosfets are inverting things
 PORTD=1<<6|1<<5|1<<4|1|1<<1; //PD2 & PD3 are the USB
}

volatile char pos0,pos1;
//timer8: 12000000/1024/23
//Then goes interrupt, then goes soft


void timer8_wait()
{
    OCR0A=0xdb;
    OCR0B=0xdb;
    TCCR0A = (1<<WGM01)|(1<<COM0B0)|(1<<COM0A0);
    TCCR0B = (1<<CS02)|(1<<CS00);  
    PORTB &= ~(1<<2); //set pins low.
    PORTB &= ~(1<<5); // from both ports
}

void timer8_pwm()
{
    OCR0A=pos0;
    OCR0B=pos1;
    TCCR0A = (1<<COM0A1)|(1<<COM0B1)|(1<<WGM01)|(1<<WGM00); 
    TCCR0B = (1<<CS01)|(1<<CS00);  
}

void timer8_toggle_mode()
{
    
    if (TCCR0A & (1<<WGM00))
    {
    //delay mode
    
    //outlet_off(1);
    }else
    {
    //PWM mode
    OCR0A=pos0;
    OCR0B=pos1;
    TCCR0A = (1<<COM0A1)|(1<<COM0B1)|(1<<WGM01)|(1<<WGM00); 
    TCCR0B = (1<<CS01)|(1<<CS00);  
    //outlet_on(1);
    //*/
    }
}

ISR(TIMER0_OVF_vect)
{
  //timer8_toggle_mode();  
  //outlet_toggle(1);
  timer8_wait();
}

ISR(TIMER0_COMPA_vect)
{
  //timer8_toggle_mode();  
  //outlet_on(1);
  timer8_pwm();
 
}


ANTARES_INIT_LOW(servotool_init_timer8)
{
    TIMSK = (1<<TOIE0)|(1<<OCIE0A);;
    //timer8_toggle_mode();
    pos0=pos1=0x6e;
    outlet_off(1);
    timer8_wait();
    servo_on(1);
    sei();
    servo_on(2);
    servo_on(3);
    
    for (pos0=0x0;pos0<0xFF; pos0++)
    {
      _delay_ms(100);
    }
    
}

//Init the 16-bit timer for pwm
//We get here some nice presision. 
ANTARES_INIT_LOW(servotool_init_timer16)
{
    //pr: 12000000/8
    // NEED 50Hz
    
     // OC1A, OC1B outputs 
    
    // TOP, set for 50Hz (20ms) 
    ICR1 = 30000; 
    // Center outputs (1.5ms) 
    OCR1A = 1500; 
    OCR1B = 1500; 
    // Timer 1 fast PWM mode 14 
    // Clear on compare, set at TOP 
    // /8 prescaler 
    TCCR1A = (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11); 
    TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS11);
    
}

