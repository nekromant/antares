// TODO: Legal stuff goes here
// This file is automagically generated, dude
// I wouldn't touch that if I were you, since all the stuff you'll fix
// Will eventually go down the closet
#include <arch/antares.h>
#include "include/adc.h"
#define CHAN_COUNT 16

static PROGMEM char channels[CHAN_COUNT]= { 
1,  2,  3,  4,  5,  6,  7,  8, 9, 10, 11,
0x00 };

static int ch;
static char channel_data[9];
ANTARES_INIT_LOW(adc_init)
{
    ADCSRA|= (1<< ADPS2) | (1<<ADPS1) | (1<<ADPS0) | (1<<ADIE) | (1 << ADEN); //125KHZ
    ADMUX |= (1<<REFS0) | (1 << ADLAR);
    ADCSRA|=(1<<ADSC);

} // __inline void adc_init()
ISR(ADC_vect) {
    channel_data[ch]=ADCH;
    switch(ch) {
    }//switch(ch)
    ADMUX&=~(0x1f);
    ADMUX|=(0xe0 & pgm_read_byte(channels+ch));
    ADCSRB|=(MUX5 & pgm_read_byte(channels+ch));
    if (ch!=CHAN_COUNT)
        ++ch;
    else
        ch=0;
    ADCSRA|=(1<<ADSC);
}
__inline uint8_t adc_get_data(int channel)
{
    return channel_data[channel];
} //
