//*****************************************************************************
// File Name	: lcdtest.c
// 
// Title		: example usage of some avr library functions
// Revision		: 1.0
// Notes		:	
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
// 
// Revision History:
// When			Who			Description of change
// -----------	-----------	-----------------------
// 30-Apr-2002	pstang		Created the program
//*****************************************************************************

//----- Include Files ---------------------------------------------------------
#include <avr/io.h>		// include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>	// include interrupt support

#include "global.h"		// include our global settings
#include "uart.h"		// include uart function library
#include "rprintf.h"	// include printf function library
#include "a2d.h"		// include A/D converter function library
#include "timer.h"		// include timer function library (timing, PWM, etc)
#include "lcd.h"

//----- Begin Code ------------------------------------------------------------
int main(void)
{
	u08 a=0;

	// initialize our libraries
	// initialize the UART (serial port)
	uartInit();
	// make all rprintf statements use uart for output
	rprintfInit(uartSendByte);
	// turn on and initialize A/D converter
	a2dInit();
	// initialize the timer system
	timerInit();
	// print a little intro message so we know things are working
	rprintf("\r\nWelcome to AVRlib!\r\n");
	
	// initialize LCD
	lcdInit();
	// direct printf output to LCD
	rprintfInit(lcdDataWrite);

	// print message on LCD
	rprintf("Welcome to AVRlib!");

	DDRA = 0x00;
	PORTA = 0x00;

	// display a bargraph of the analog voltages on a2d channels 0,1
	while(1)
	{
		lcdGotoXY(0,0);
		lcdProgressBar(a2dConvert8bit(0), 255, 20);
		rprintf(" X: %d", a2dConvert8bit(0));
		rprintf(" Sample: %d", a++);
		lcdGotoXY(0,1);
		lcdProgressBar(a2dConvert8bit(1), 255, 20);
		rprintf(" Y: %d", a2dConvert8bit(1));
	}

	return 0;
}
