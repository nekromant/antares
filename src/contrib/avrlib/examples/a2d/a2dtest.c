//*****************************************************************************
// File Name	: a2dtest.c
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
// 20-Oct-2002	pstang		Created the program
//*****************************************************************************
 
//----- Include Files ---------------------------------------------------------
#include <avr/io.h>		// include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>	// include interrupt support

#include "global.h"		// include our global settings
#include "uart.h"		// include uart function library
#include "rprintf.h"	// include printf function library
#include "timer.h"		// include timer function library (timing, PWM, etc)
#include "a2d.h"		// include A/D converter function library
#include "vt100.h"		// include VT100 terminal support

//----- Begin Code ------------------------------------------------------------
int main(void)
{
	u16 a=0;
	u08 i=0;

	// initialize our libraries
	// initialize the UART (serial port)
	uartInit();
	// make all rprintf statements use uart for output
	rprintfInit(uartSendByte);
	// initialize the timer system
	timerInit();
	// turn on and initialize A/D converter
	a2dInit();

	// print a little intro message so we know things are working
	vt100ClearScreen();
	vt100SetCursorPos(1,1);
	rprintf("Welcome to the a2d test!\r\n");
	
	// configure a2d port (PORTA) as input
	// so we can receive analog signals
	DDRA = 0x00;
	// make sure pull-up resistors are turned off
	PORTA = 0x00;

	// set the a2d prescaler (clock division ratio)
	// - a lower prescale setting will make the a2d converter go faster
	// - a higher setting will make it go slower but the measurements
	//   will be more accurate
	// - other allowed prescale values can be found in a2d.h
	a2dSetPrescaler(ADC_PRESCALE_DIV32);

	// set the a2d reference
	// - the reference is the voltage against which a2d measurements are made
	// - other allowed reference values can be found in a2d.h
	a2dSetReference(ADC_REFERENCE_AVCC);

	// use a2dConvert8bit(channel#) to get an 8bit a2d reading
	// use a2dConvert10bit(channel#) to get a 10bit a2d reading

	while(1)
	{
		// sample all a2d channels and print them to the terminal
		vt100SetCursorPos(2,1);
		for(i=0; i<8; i++)
		{
			rprintf("Channel %d: %d   \r\n", i, a2dConvert8bit(i));
		}
		// print the sample number so far
		rprintf("Sample # : %d   \r\n", a++);
	}
	
	return 0;
}
