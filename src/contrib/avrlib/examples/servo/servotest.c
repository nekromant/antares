//*****************************************************************************
// File Name	: servotest.c
// 
// Title		: example usage of RC servo library functions
// Revision		: 1.0
// Notes		:	
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
// 
// Revision History:
// When			Who			Description of change
// -----------	-----------	-----------------------
// 10-Sep-2002	pstang		Created the program
//*****************************************************************************


//----- Include Files ---------------------------------------------------------
#include <avr/io.h>		// include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>	// include interrupt support

#include "global.h"		// include our global settings
#include "uart.h"		// include uart function library
#include "rprintf.h"	// include printf function library
#include "a2d.h"		// include A/D converter function library
#include "timer.h"		// include timer function library (timing, PWM, etc)
#include "servo.h"		// include RC servo driver library
#include "vt100.h"		// include VT100 terminal library

void servoTest(void);

//----- Begin Code ------------------------------------------------------------
int main(void)
{
	// initialize our libraries
	// initialize the UART (serial port)
	uartInit();
	// set the baud rate of the UART for our debug/reporting output
	uartSetBaudRate(9600);
	// set uartSendByte as the output for all rprintf statements
	rprintfInit(uartSendByte);
	// initialize the timer system
	timerInit();
	// initialize vt100 library
	vt100Init();
	vt100ClearScreen();
	// print a little intro message so we know things are working
	rprintf("\r\nWelcome to Servo Test!\r\n");

	// begin servo test
	servoTest();

	return 0;
}

void servoTest(void)
{
	u08 pos;
	u08 channel;

	// do some examples
	// initialize RC servo system
	servoInit();
	// setup servo output channel-to-I/Opin mapping
	// format is servoSetChannelIO( CHANNEL#, PORT, PIN );
	servoSetChannelIO(0, _SFR_IO_ADDR(PORTC), PC0);
	servoSetChannelIO(1, _SFR_IO_ADDR(PORTC), PC1);
	servoSetChannelIO(2, _SFR_IO_ADDR(PORTC), PC2);
	servoSetChannelIO(3, _SFR_IO_ADDR(PORTC), PC3);

	// set port pins to output
	outb(DDRC, 0x0F);

	pos = 0;

#define SPEED_SERVO	1

	// spin servos sequentially back and forth between their limits
	while(1)
	{
		for(channel=0; channel<SERVO_NUM_CHANNELS; channel++)
		{
			for(pos=0; pos<SERVO_POSITION_MAX; pos++)
			{
				servoSetPosition(channel,pos);
				timerPause(SPEED_SERVO);
			}
		}

		for(channel=0; channel<SERVO_NUM_CHANNELS; channel++)
		{
			for(pos=SERVO_POSITION_MAX; pos>=1; pos--)
			{
				servoSetPosition(channel,pos);
				timerPause(SPEED_SERVO);
			}
		}
	}
}
