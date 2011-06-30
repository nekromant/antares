//*****************************************************************************
// File Name	: ads7870test.c
// 
// Title		: example usage of ADS7870 library functions
// Revision		: 1.0
// Notes		:	
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
// 
// Revision History:
// When			Who			Description of change
// -----------	-----------	-----------------------
// 07-Jul-2005	pstang		Created the program
//*****************************************************************************
 
//----- Include Files ---------------------------------------------------------
#include <avr/io.h>		// include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>	// include interrupt support

#include "global.h"		// include our global settings
#include "uart.h"		// include uart function library
#include "rprintf.h"	// include printf function library
#include "timer.h"		// include timer function library (timing, PWM, etc)
#include "vt100.h"		// include VT100 terminal support
#include "spi.h"		// include spi support
#include "ads7870.h"	// include ADS7828 support

void ads7870test(void);

//----- Begin Code ------------------------------------------------------------
int main(void)
{
	// initialize our libraries
	// initialize the UART (serial port)
	uartInit();
	uartSetBaudRate(115200);
	// make all rprintf statements use uart for output
	rprintfInit(uartSendByte);
	// initialize the timer system
	timerInit();
	// clear terminal screen
	vt100ClearScreen();
	vt100SetCursorPos(1,1);
	// print a little intro message so we know things are working
	rprintf("\r\nWelcome to ADS7870 test!\r\n");
	
	// run tests
	ads7870test();

	return 0;
}

void ads7870test(void)
{
	u08 i;
	u16 conv=0;

	// initialize a/d converter
	if(ads7870Init())
	{
		rprintf("ADS7870 detected and initialized!\r\n");
	}
	else
	{
		rprintf("Cannot detect ADS7870!\r\n");
		return;
	}
	
	while(1)
	{
		// position cursor
		vt100SetCursorPos(4,1);

		for(i=0; i<8; i++)
		{
			// convert
			conv = ads7870Convert(i);
			// print results
			rprintf("CH#%d: Conversion result is: %d    \r\n", i, conv);
		}
		// pause between readings
		timerPause(100);
	}
}
