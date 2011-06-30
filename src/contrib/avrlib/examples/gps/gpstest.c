//*****************************************************************************
// File Name	: gpstest.c
// 
// Title		: example usage of gps processing library functions
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
//#include <math.h>
#include <stdlib.h>

#include "global.h"		// include our global settings
#include "uart2.h"		// include dual-uart function library
#include "rprintf.h"	// include printf function library
#include "timer.h"		// include timer function library (timing, PWM, etc)
#include "gps.h"		// include gps data support
#include "tsip.h"		// include TSIP gps packet handling
#include "nmea.h"		// include NMEA gps packet handling
#include "vt100.h"		// include VT100 terminal commands

// uartRxOverflow is a global variable defined in uart.c/uart2.c
// we define it here as <extern> here so that we can use its value
// in code contained in this file
extern unsigned short uartRxOverflow[2];

void gpsTsipTest(void);
void gpsNmeaTest(void);

//----- Begin Code ------------------------------------------------------------
int main(void)
{
	// initialize our libraries
	// initialize the UART (serial port)
	uartInit();
	// set the baud rate of UART 0 for our debug/reporting output
	uartSetBaudRate(0,9600);
	// set uart0SendByte as the output for all rprintf statements
	rprintfInit(uart0SendByte);
	// initialize the timer system
	timerInit();
	// initialize vt100 library
	vt100Init();
	
	// print a little intro message so we know things are working
	vt100ClearScreen();
	rprintf("\r\nWelcome to GPS Test!\r\n");
	
	// run example gps processing loop
	// (pick the one appropriate for your GPS packet format)
//	gpsTsipTest();
	gpsNmeaTest();
	
	return 0;
}

void gpsTsipTest(void)
{
	// set the baud rate of UART 1 for TSIP
	uartSetBaudRate(1,9600);

	// clear screen
	vt100ClearScreen();
	// initialize gps library
	gpsInit();
	// initialize gps packet decoder 
	tsipInit(uart1SendByte);	// use uart1 for tsip packet output

	// begin gps packet processing loop
	while(1)
	{
		// process received gps packets until receive buffer is exhausted
		while( tsipProcess(uartGetRxBuffer(1)) );

		// set cursor position to top left of screen
		vt100SetCursorPos(0,0);
		// print/dump current formatted GPS data
		gpsInfoPrint();
		// print UART 1 overflow status to verify that we're processing packets
		// fast enough and that our receive buffer is large enough
		rprintf("Uart1RxOvfl: %d\r\n",uartRxOverflow[1]);
		// pause for 100ms
		timerPause(100);
	}
}

void gpsNmeaTest(void)
{
	// set the baud rate of UART 1 for NMEA
	uartSetBaudRate(1,4800);

	// clear screen
	vt100ClearScreen();
	// initialize gps library
	gpsInit();
	// initialize gps packet decoder 
	nmeaInit();

	// begin gps packet processing loop
	while(1)
	{
		// process received gps packets until receive buffer is exhausted
		while( nmeaProcess(uartGetRxBuffer(1)) );

		// set cursor position to top left of screen
		vt100SetCursorPos(0,0);
		// print/dump current formatted GPS data
		gpsInfoPrint();
		// print UART 1 overflow status to verify that we're processing packets
		// fast enough and that our receive buffer is large enough
		rprintf("Uart1RxOvfl: %d\r\n",uartRxOverflow[1]);
		// pause for 100ms
		timerPause(100);
	}
}

