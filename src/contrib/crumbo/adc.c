// TODO: Legal stuff goes here
// This file is automagically generated, dude
// I wouldn't touch that if I were you, since all the stuff you'll fix
// Will eventually go down the closet
#include <arch/antares.h>
#include "include/adc.h"
#define _DBG_ENABLED 1
#include "include/debug.h"

#define CHAN_COUNT 16

static PROGMEM char channels[CHAN_COUNT]= { 
0,  1,  2,  3,  4,  5,  6,  7, 8, 9, 10, 11,
0x00 };

static volatile int ch;
static volatile uint16_t channel_data[CHAN_COUNT];

#define VREFS (1<<REFS0)

ANTARES_INIT_LOW(adc_init)
{
    ADCSRA|= (1<< ADPS2) | (1<<ADPS1) | (1<<ADPS0) | (1<<ADIE) | (1 << ADEN); //125KHZ
    ADMUX |= VREFS;
    ADCSRA|=(1<<ADSC);
    //ADCSRA|=(1<<ADATE);

} // __inline void adc_init()
ISR(ADC_vect) {
    channel_data[ch]=ADCW;
    //switch(ch) {
    //}//switch(ch)
    ADMUX&=~(0x1f);
    if (ch<CHAN_COUNT)
        ++ch;
    else
        ch=0;
    ADMUX=(VREFS | pgm_read_byte(channels+ch));
    ADCSRB|=(MUX5 & pgm_read_byte(channels+ch));
    ADCSRA|=(1<<ADSC);
}
__inline uint8_t adc_get_data(int channel)
{
    return channel_data[channel];
} //


#ifdef CONFIG_CONTRIB_CRUMBO_ADC_TEST

ANTARES_APP(adc_test)
{
 int i;
 DBG("ADC data dump");
 for (i=0; i< CHAN_COUNT; i++)
 {
   dump16(channel_data[i]);
 }
 _delay_ms(1000);
}
#endif
