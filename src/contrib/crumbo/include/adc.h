// TODO: Legal stuff goes here
// This file is automagically generated, dude
// I wouldn't touch that if I were you, since all the stuff you'll fix
// Will eventually go down the closet
#ifndef __ADC_H
#define __ADC_H

//Now go the prototypes
__inline void process_dithered(volatile uint16_t* data);
__inline uint8_t adc_get_data(int channel);
#define RGB_R	1
#define RGB_L	2
#define RANGE_BACK	3
#define RANGE_FWD	4
#define RANGE_LEFT_US	5
#define RANGE_RIGHT_US	6
#define RANGE_RIGHT_SIDE	7
#define RANGE_LEFT_SIDE	8
#endif // ADC_H
