//*****************************************************************************
// File Name	: encodertest.c
// 
// Title		: example usage of encoder driver functions
// Revision		: 1.0
// Notes		:
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
// 
// Revision History:
// When			Who			Description of change
// -----------	-----------	-----------------------
// 27-Jan-2003	pstang		Created the program
//*****************************************************************************


//----- Include Files ---------------------------------------------------------
#include <avr/io.h>		// include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>	// include interrupt support

#include "global.h"		// include our global settings
#include "uart.h"		// include uart function library
#include "rprintf.h"	// include printf function library
#include "timer.h"		// include timer function library (timing, PWM, etc)
#include "vt100.h"		// include VT100 terminal support
#include "encoder.h"	// include encoder driver

void encoderTest(void);

//----- Begin Code ------------------------------------------------------------
int main(void)
{
	// initialize our libraries
	// initialize the UART (serial port)
	uartInit();
	// set the baud rate of the UART for our debug/reporting output
	uartSetBaudRate(9600);
	// initialize the timer system
	timerInit();
	// initialize rprintf system
	rprintfInit(uartSendByte);
	// initialize vt100 library
	vt100Init();

	// clear the terminal screen
	vt100ClearScreen();
	
	// run the test
	encoderTest();

	return 0;
}

void encoderTest(void)
{
	// initialize the encoders
	encoderInit();

	// print a little intro message so we know things are working
	rprintf("\r\nWelcome to the encoder test!\r\n");

	// report encoder position continuously
	while(1)
	{
		rprintfProgStrM("Encoder0: ");
		// print encoder0 position
		// use base-10, 10 chars, signed, pad with spaces
		rprintfNum(10, 10, TRUE, ' ', encoderGetPosition(0));

		rprintfProgStrM("  Encoder1: ");
		// print encoder1 position
		// use base-10, 10 chars, signed, pad with spaces
		rprintfNum(10, 10, TRUE, ' ', encoderGetPosition(1));

		// print carriage return and line feed
		rprintfCRLF();
	}
}

