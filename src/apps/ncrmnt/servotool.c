#include <arch/antares.h>
#include "include/servotool.h"

//Init the pins
//We do not use macros here, since it will be less code for uC this way
ANTARES_INIT_LOW(servotool_init_pins)
{
 
 DDRB=0xFF; 
 DDRD=1<<6|1<<5|1<<4|1|1<<1; //PD2 & PD3 are the USB
 //Power initially off
 PORTB=0xFF; //Mosfets are inverting things
 PORTD=1<<6|1<<5|1<<4|1|1<<1; //PD2 & PD3 are the USB
}

//timer8: 12000000/1024/23
//Then goes interrupt, then goes soft


//Init the 16-bit timer for pwm
//We get here some nice presision. 
ANTARES_INIT_LOW(servotool_init_timer16)
{
    //pr: 12000000/8
    // NEED 50Hz
    
     // OC1A, OC1B outputs 
    DDRB |= (1<<PB4)|(1<<PB3); 
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

