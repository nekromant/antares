//*****************************************************************************
// File Name	: cmdlinetest.c
// 
// Title		: example usage of cmdline (command line) functions
// Revision		: 1.0
// Notes		:	
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
// 
// Revision History:
// When			Who			Description of change
// -----------	-----------	-----------------------
// 21-Jul-2003	pstang		Created the program
//*****************************************************************************


//----- Include Files ---------------------------------------------------------
#include <avr/io.h>			// include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>	// include interrupt support

#include "global.h"		// include our global settings
#include "uart.h"		// include uart function library
#include "rprintf.h"	// include printf function library
#include "a2d.h"		// include A/D converter function library
#include "timer.h"		// include timer function library (timing, PWM, etc)
#include "vt100.h"		// include vt100 terminal support
#include "cmdline.h"	// include cmdline function library

// global variables
u08 Run;

// functions
void goCmdline(void);
void exitFunction(void);
void helpFunction(void);
void dumpArgsStr(void);
void dumpArgsInt(void);
void dumpArgsHex(void);

//----- Begin Code ------------------------------------------------------------
int main(void)
{
	// initialize our libraries
	// initialize the UART (serial port)
	uartInit();
	uartSetBaudRate(9600);
	// make all rprintf statements use uart for output
	rprintfInit(uartSendByte);
	// turn on and initialize A/D converter
	a2dInit();
	// initialize the timer system
	timerInit();
	// initialize vt100 terminal
	vt100Init();

	// configure port B for led output and pushbutton input
	outb(DDRB, 0x0F);
	// all LEDs on
	outb(PORTB, 0x00);
	// wait for hardware to power up
	timerPause(100);
	// all LEDs off
	outb(PORTB, 0x0F);

	// start command line
	goCmdline();

	return 0;
}

void goCmdline(void)
{
	u08 c;

	// print welcome message
	vt100ClearScreen();
	vt100SetCursorPos(1,0);
	rprintfProgStrM("\r\nWelcome to the Command Line Test Suite!\r\n");

	// initialize cmdline system
	cmdlineInit();

	// direct cmdline output to uart (serial port)
	cmdlineSetOutputFunc(uartSendByte);

	// add commands to the command database
	cmdlineAddCommand("exit",		exitFunction);
	cmdlineAddCommand("help",		helpFunction);
	cmdlineAddCommand("dumpargs1",	dumpArgsStr);
	cmdlineAddCommand("dumpargs2",	dumpArgsInt);
	cmdlineAddCommand("dumpargs3",	dumpArgsHex);

	// send a CR to cmdline input to stimulate a prompt
	cmdlineInputFunc('\r');

	// set state to run
	Run = TRUE;

	// main loop
	while(Run)
	{
		// pass characters received on the uart (serial port)
		// into the cmdline processor
		while(uartReceiveByte(&c)) cmdlineInputFunc(c);

		// run the cmdline execution functions
		cmdlineMainLoop();
	}

	rprintfCRLF();
	rprintf("Exited program!\r\n");
}

void exitFunction(void)
{
	// to exit, we set Run to FALSE
	Run = FALSE;
}

void helpFunction(void)
{
	rprintfCRLF();

	rprintf("Available commands are:\r\n");
	rprintf("help      - displays available commands\r\n");
	rprintf("dumpargs1 - dumps command arguments as strings\r\n");
	rprintf("dumpargs2 - dumps command arguments as decimal integers\r\n");
	rprintf("dumpargs3 - dumps command arguments as hex integers\r\n");

	rprintfCRLF();
}

void dumpArgsStr(void)
{
	rprintfCRLF();
	rprintf("Dump arguments as strings\r\n");

	rprintfProgStrM("Arg0: "); rprintfStr(cmdlineGetArgStr(0)); rprintfCRLF();
	rprintfProgStrM("Arg1: "); rprintfStr(cmdlineGetArgStr(1)); rprintfCRLF();
	rprintfProgStrM("Arg2: "); rprintfStr(cmdlineGetArgStr(2)); rprintfCRLF();
	rprintfProgStrM("Arg3: "); rprintfStr(cmdlineGetArgStr(3)); rprintfCRLF();
	rprintfCRLF();
}

void dumpArgsInt(void)
{
	rprintfCRLF();
	rprintf("Dump arguments as integers\r\n");

	// printf %d will work but only if your numbers are less than 16-bit values
	//rprintf("Arg1 as int: %d\r\n", cmdlineGetArgInt(1));
	//rprintf("Arg2 as int: %d\r\n", cmdlineGetArgInt(2));
	//rprintf("Arg3 as int: %d\r\n", cmdlineGetArgInt(3));

	// printfNum is good for longs too
	rprintf("Arg1 as int: "); rprintfNum(10, 10, TRUE, ' ', cmdlineGetArgInt(1)); rprintfCRLF();
	rprintf("Arg2 as int: "); rprintfNum(10, 10, TRUE, ' ', cmdlineGetArgInt(2)); rprintfCRLF();
	rprintf("Arg3 as int: "); rprintfNum(10, 10, TRUE, ' ', cmdlineGetArgInt(3)); rprintfCRLF();
	rprintfCRLF();
}

void dumpArgsHex(void)
{
	rprintfCRLF();
	rprintf("Dump arguments as hex integers\r\n");

	rprintf("Arg1 as hex: "); rprintfNum(16, 8, FALSE, ' ', cmdlineGetArgHex(1)); rprintfCRLF();
	rprintf("Arg2 as hex: "); rprintfNum(16, 8, FALSE, ' ', cmdlineGetArgHex(2)); rprintfCRLF();
	rprintf("Arg3 as hex: "); rprintfNum(16, 8, FALSE, ' ', cmdlineGetArgHex(3)); rprintfCRLF();
	rprintfCRLF();
}
