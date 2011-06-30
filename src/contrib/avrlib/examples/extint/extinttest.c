//*****************************************************************************
// File Name	: extinttest.c
// 
// Title		: example usage of external interrupt library functions
// Revision		: 1.0
// Notes		:	
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
// 
// Revision History:
// When			Who			Description of change
// -----------	-----------	-----------------------
// 14-Dec-2004	pstang		Created the program
//*****************************************************************************


//----- Include Files ---------------------------------------------------------
#include <avr/io.h>			// include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>	// include interrupt support

#include "global.h"		// include our global settings
#include "uart.h"		// include uart function library
#include "rprintf.h"	// include printf function library
#include "timer.h"		// include timer function library
#include "extint.h"		// include external interrupt library

// global variables
volatile u16 Int0Count;
volatile u16 Int1Count;

// functions
void extintTest(void);
void myInt0Handler(void);
void myInt1Handler(void);

//----- Begin Code ------------------------------------------------------------
int main(void)
{
	// initialize our libraries
	// initialize the UART (serial port)
	uartInit();
	// set the baud rate of the UART for our debug/reporting output
	uartSetBaudRate(115200);
	// initialize rprintf system
	rprintfInit(uartSendByte);
	// initialize timers
	timerInit();
	
	// run the test
	extintTest();

	return 0;
}

void extintTest(void)
{
	u16 temp0, temp1;

	// print a little intro message so we know things are working
	rprintf("\r\n\n\nWelcome to the External Interrupt library test program!\r\n");
	
	// initialize the external interrupt library
	rprintf("Initializing external interrupt library\r\n");
	extintInit();

	// configure external interrupts for rising-edge triggering.
	// when a rising-edge pulse arrives on INT0 or INT1,
	// the interrupt will be triggered
	rprintf("Configuring external interrupts\r\n");
	extintConfigure(EXTINT0, EXTINT_EDGE_RISING);
	extintConfigure(EXTINT1, EXTINT_EDGE_RISING);

	// attach user interrupt routines.
	// when the interrupt is triggered, the user routines will be executed
	rprintf("Attaching user interrupt routines\r\n");
	extintAttach(EXTINT0, myInt0Handler);
	extintAttach(EXTINT1, myInt1Handler);

	// enable the interrupts
	rprintf("Enabling external interrupts\r\n");
	// (support for this has not yet been added to the library)
	sbi(GIMSK, INT0);
	sbi(GIMSK, INT1);

	// In this loop we will count the number of external interrupts,
	// and therefore the number of rising edges, that occur in one second.
	// This is precisely the frequency, in cycles/second or Hz, of the signal
	// that is triggering the interrupt.

	while(1)
	{
		// reset interrupt counters
		Int0Count = 0;
		Int1Count = 0;
		// wait 1 second
		timerPause(1000);
		// get counter values
		temp0 = Int0Count;
		temp1 = Int1Count;
		// print results
		rprintf("Frequency on INT0 pin: %dHz -- On INT1 pin: %dHz\r\n", temp0, temp1);
	}
}

void myInt0Handler(void)
{
	// count this interrupt event
	Int0Count++;
}

void myInt1Handler(void)
{
	// count this interrupt event
	Int1Count++;
}

