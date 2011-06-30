//*****************************************************************************
// File Name	: timertest.c
// 
// Title		: example usage of timer library functions
// Revision		: 1.0
// Notes		:	
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
// 
// Revision History:
// When			Who			Description of change
// -----------	-----------	-----------------------
// 30-Apr-2003	pstang		Created the program
//*****************************************************************************


//----- Include Files ---------------------------------------------------------
#include <avr/io.h>			// include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>	// include interrupt support

#include "global.h"		// include our global settings
#include "uart.h"		// include uart function library
#include "rprintf.h"	// include printf function library
#include "timer.h"		// include timer function library (timing, PWM, etc)

void timerTest(void);

//----- Begin Code ------------------------------------------------------------
int main(void)
{
	// initialize our libraries
	// initialize the UART (serial port)
	uartInit();
	// set the baud rate of the UART for our debug/reporting output
	uartSetBaudRate(9600);
	// initialize rprintf system
	rprintfInit(uartSendByte);
	
	// run the test
	timerTest();

	return 0;
}

void timerTest(void)
{
	// print a little intro message so we know things are working
	rprintf("\r\n\n\nWelcome to the timer library test program!\r\n");

	// initialize the timer system
	timerInit();

	// to use the internal timers to produce a calibrated delay,
	// use the timerPause() function.  timerPause takes a 16-bit
	// integer argument in milliseconds

	// example: wait for 1/2 of a second, or 500ms
	rprintf("\r\nTest of timerPause() function\r\n");
	rprintf("Here comes a 1/2-second delay...\r\n");
	timerPause(500);
	rprintf("Done!\r\n");


	// here's an example of using the timer library to do
	// pulse-width modulation or PWM.  PWM signals can be created on
	// any output compare (OCx) pin.  See your processor's data sheet
	// for more information on which I/O pins have output compare
	// capability.
	rprintf("\r\nTest of timer1 PWM output\r\n");

	// set the OC1x port pins to output
	// We need to do this so we can see and use the PWM signal
	// ** these settings are correct for most processors, but not for all
	sbi(DDRD, PD4);
	sbi(DDRD, PD5);

	// initialize timer1 for PWM output
	// - you may use 8,9, or 10 bit PWM resolution
	rprintf("Initializing timer1 for PWM\r\n");
	timer1PWMInit(8);

	// turn on the channel A PWM output of timer1
	// - this signal will come out on the OC1A I/O pin
	rprintf("Turning on timer1 channel A PWM output\r\n");
	timer1PWMAOn();

	// set the duty cycle of the channel A output
	// - let's try 25% duty, or 256*25% = 64
	rprintf("Setting duty cycle to 25%%\r\n");
	timer1PWMASet(64);

	// turn on channel B and set it to 75% duty cycle
	rprintf("Turning on channel B too, with 75%% duty\r\n");
	timer1PWMBOn();
	timer1PWMBSet(192);

	// wait for 5 seconds
	rprintf("Pause for 5 seconds...\r\n");
	timerPause(5000);

	// now turn off all PWM on timer1
	rprintf("Turning off all PWM on timer1\r\n");
	timer1PWMOff();
}

