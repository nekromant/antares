#include <arch/antares.h>



//Buttons: B0 D6 D4 D5
//DRVz: B1 2 3 4


#define	DRIVER_PINS	(1<<1)|(1<<2)|(1<<3)|(1<<4)
#define	DRIVER_PORT	PORTB

ANTARES_INIT_LOW(hw_init)
{
  DDRB=(1<<1)|(1<<2)|(1<<3)|(1<<4);
  DDRD&=~(1<<6);
  PORTD=0xff;
  _delay_ms(1500);
  PORTD=0;
  
}

ANTARES_INIT_LOW(init_timer8)
{
  //TCCR0A = (1<<COM0A1)|(1<<COM0B1)|(1<<WGM01)|(1<<WGM00); 
  //TCCR0B = (1<<CS01)|(1<<CS00);     
  DDRB=0xff;
  PORTB=0xff;
  
}


ANTARES_APP(bot_app)
{
  PORTD=0xff;
  _delay_ms(1500);
  while(1){
    OCR0A=0x66;
    OCR0B=0x66;
  PORTD^=0xff;
  _delay_ms(500);
  }
}
