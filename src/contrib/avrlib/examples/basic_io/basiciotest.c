//*****************************************************************************
// File Name	: basiciotest.c
// 
// Title		: example usage of basic input and output functions on the AVR
// Revision		: 1.0
// Notes		:
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
// 
// Revision History:
// When			Who			Description of change
// -----------	-----------	-----------------------
// 02-Feb-2003	pstang		Created the program
//*****************************************************************************

 
//----- Include Files ---------------------------------------------------------
#include <avr/io.h>		// include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>	// include interrupt support

#include "global.h"		// include our global settings
//#include "uart.h"		// include uart function library
//#include "rprintf.h"	// include printf function library
//#include "timer.h"		// include timer function library (timing, PWM, etc)
//#include "vt100.h"		// include VT100 terminal support
//#include "encoder.h"	// include encoder driver


//----- Begin Code ------------------------------------------------------------
int main(void)
{
	// initialize our libraries
	// initialize the UART (serial port)
//	uartInit();
	// set the baud rate of the UART for our debug/reporting output
//	uartSetBaudRate(9600);
	// initialize the timer system
//	timerInit();
	// initialize rprintf system
//	rprintfInit(uartSendByte);
	// initialize vt100 library
//	vt100Init();

	// clear the terminal screen
//	vt100ClearScreen();

	u08 a;

	// All AVR processors have I/O ports which each contain up to 8
	// user-controllable pins.  From a hardware perspective, these I/O pins
	// are each an actual physical pin coming out of the processor chip.
	// The voltage on the pins can be sensed or controlled via software,
	// hence their designation as Input/Output pins.

	// While I/O pins are actual wires in the real world, they also exist
	// inside the processor as special memory locations called registers.
	// The software-controlled contents of these registers is what
	// determines the state and operation of I/O pins and I/O ports.

	// Since AVR processors deal naturally with 8 bits at a time, I/O pins
	// are grouped into sets of 8 to form I/O ports.  Three registers
	// are assigned to each I/O port to control the function and state of
	// that port's pins.  The registers are 8-bits wide, and each bit (#0-7)
	// determines the operation of the corresponding number pin (pin 0-7).
	// The three registers are:

	// DDRx  - this register determines the direction (input/output) of the pins on port[x]
	//			A '0' bit in the DDR makes that port pin act as input
	//			A '1' bit in the DDR makes that port pin act as output

	// PORTx - this register contains the output state of the pins on port[x]
	//			A '0' bit makes the port pin output a LOW  (~0V)
	//			A '1' bit makes the port pin output a HIGH (~5V)

	// PINx  - this register contains the input state of the pins on port[x]
	//			A '0' bit indicates that the port pin is LOW  (at ~0V)
	//			A '1' bit indicates that the port pin is HIGH (at ~5V)

	// The x should be replaced with A,B,C,D,E,F, or G depending on the
	// desired port.  Note that not all AVR processors have the same set
	// or number of ports.  Consult the datasheet for your specific processor
	// to find out which ports it has.

	// in the AVR-GCC C language, ports can be accessed using two kinds of
	// commands:
	// inb() and outb()		-	in-byte and out-byte
	// cbi() and sbi()		-	clear-bit and set-bit

	// inb() and outb() should be used when you intend to read or write
	// several bits of a register at once.  Here are some examples:

	outb(DDRA, 0x00);	// set all port A pins to input
	a = inb(PINA);		// read the input state of all pins on port A
	
	outb(DDRB, 0xFF);	// set all port B pins to output
	outb(PORTB, 0xF0);	// set PB4-7 to HIGH and PB0-3 to LOW
	
	// Often you may wish to change only a single bit in the registers
	// while leaving the rest unaltered.  For this, use cbi() and sbi().
	// For example:

	sbi(DDRC, 0);		// sets PC0 to be an output
	sbi(DDRC, 1);		// sets PC1 to be an output

	cbi(PORTC, 1);		// sets PC1 to output a LOW without altering any other pin
	
	// the lines below will cause PC0 to pulse twice,
	// but will leave all other port C pins unchanged
	cbi(PORTC, 0);		// sets PC0 to output a LOW
	sbi(PORTC, 0);		// sets PC0 to output a HIGH
	cbi(PORTC, 0);		// sets PC0 to output a LOW
	sbi(PORTC, 0);		// sets PC0 to output a HIGH
	cbi(PORTC, 0);		// sets PC0 to output a LOW
	
	
	return 0;
}

