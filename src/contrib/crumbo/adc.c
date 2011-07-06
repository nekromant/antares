// TODO: Legal stuff goes here
// This file is automagically generated, dude
// I wouldn't touch that if I were you, since all the stuff you'll fix
// Will eventually go down the closet
#include <arch/antares.h>
#include "include/adc.h"
#define _DBG_ENABLED 1
#include "include/debug.h"

#define CHAN_COUNT 16

static char channels[CHAN_COUNT]= { 
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

static volatile int ch;
static volatile uint16_t channel_data[CHAN_COUNT*2];

#define VREFS (1<<REFS0)

ANTARES_INIT_LOW(adc_init)
{
    //
    #ifdef CONFIG_CONTRIB_CRUMBO_ADC_LOWSPEED
    ADCSRA|= (1<< ADPS0)| (1<< ADPS1) |(1<< ADPS2) | (1<<ADIE) | (1 << ADEN); // prsc: /128
    #else
    ADCSRA|= (1<< ADPS2) | (1<<ADIE) | (1 << ADEN); //125KHZ
    #endif
    ADMUX |= VREFS;
    ADCSRA|=(1<<ADSC);
    //ADCSRA|=(1<<ADATE);

} // __inline void adc_init()

#ifdef CONFIG_CONTRIB_CRUMBO_ADC_DITH
static volatile uint16_t dith_data[CHAN_COUNT*2];
static volatile int cnt;
static char samples=50;
static __inline void dither_process()
{
  cnt++;
  int i;
  if (cnt==samples)
  {
    for (i=0; i<CHAN_COUNT;i ++)
    {
      dith_data[i]/=samples;
//        dith_data[i]/=20;
//        #warning This is shit
    }
    #ifdef CONFIG_CONTRIB_CRUMBO_ADC_DITH_TEST
    //for (i=0; i< CHAN_COUNT; i++)
    //  {
      dump16(dith_data[0]);
    //  }
    //_delay_ms(1000);
    #endif
    #warning Crap =(
      process_dithered(&dith_data[0]);
      
      
    for (i=0; i<CHAN_COUNT;i ++)
    {
      dith_data[i]=0;
//       dith_data[i]/=20;
//       #warning This is shit
    }
    cnt=0;
  }
  
  for (i=0; i<CHAN_COUNT;i ++)
  {
    dith_data[i]+=channel_data[i];
  }
}
#endif

ISR(ADC_vect) {
    channel_data[ch]=ADCW;
    //switch(ch) {
    //}//switch(ch)
    ADMUX&=~(0x1f);
    if (ch<CHAN_COUNT)
        ++ch;
    else
    {
        ch=0;
      #ifdef CONFIG_CONTRIB_CRUMBO_ADC_DITH
	dither_process();
      #endif
      #ifdef CONFIG_CONTRIB_CRUMBO_ADC_DITHSIM
	process_dithered(&channel_data[0]);
      #endif
    }
    ADMUX=(VREFS | channels[ch]);
    ADCSRB|=(MUX5 & channels[ch]);
    ADCSRA|=(1<<ADSC);
    //dump16(ch);
}
__inline uint8_t adc_get_data(int channel)
{
    return channel_data[channel];
}


#ifdef CONFIG_CONTRIB_CRUMBO_ADC_TEST

ANTARES_APP(adc_test)
{
 int i;
 DBG("ADC data dump");
 for (i=0; i< 8; i++)
 {
   dump16(channel_data[i]);
 }
 _delay_ms(100);
}
#endif
