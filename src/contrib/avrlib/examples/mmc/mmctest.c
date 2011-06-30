//*****************************************************************************
// File Name	: mmctest.c
// 
// Title		: example usage of mmc functions
// Revision		: 1.0
// Notes		:	
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
// 
// Revision History:
// When			Who			Description of change
// -----------	-----------	-----------------------
// 20-SEP-2004	pstang		Created the program
//*****************************************************************************


//----- Include Files ---------------------------------------------------------
#include <avr/io.h>			// include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>	// include interrupt support

#include "global.h"		// include our global settings
#include "uart.h"		// include uart function library
#include "rprintf.h"	// include printf function library
#include "timer.h"		// include timer function library (timing, PWM, etc)
#include "vt100.h"		// include VT100 terminal commands
#include "spi.h"		// include SPI interface functions
#include "mmc.h"		// include MMC card access functions
#include "debug.h"

void mmcTest(void);

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
	// initialize vt100 terminal
	vt100Init();

	timerPause(100);

	// print welcome message
	vt100ClearScreen();
	vt100SetCursorPos(1,0);
	rprintfProgStrM("\r\nWelcome to the MMC Test Suite!\r\n");

	timerPause(1000);

	mmcTest();

	return 0;
}

void mmcTest(void)
{
	u32 sector=0;
	u08 buffer[0x200];
	int c;

	// initialize MMC interface
	mmcInit();

	// print new prompt
	rprintf("\r\ncmd>");

	// testing loop
	while(1)
	{
		// check for keypress
		if((c=uartGetByte()) != -1)
		{
			switch(c)
			{
			case 'i':
				// initialize card 
				rprintf("\r\nResetting MMC/SD Card\r\n");
				mmcReset();
				break;
			case 'r':
				// read current sector into buffer
				rprintf("\r\nRead Sector %d\r\n", sector);
				mmcRead(sector, buffer);
				// print buffer contents
				debugPrintHexTable(0x200, buffer);
				break;
			case 'w':
				// write current sector with data from buffer
				rprintf("\r\nWrite Sector %d\r\n", sector);
				mmcWrite(sector, buffer);
				break;
			// move to new sector
			case '+': sector++;		rprintf("\r\nSector = %d", sector); break;
			case '-': sector--;		rprintf("\r\nSector = %d", sector); break;
			case '*': sector+=512;	rprintf("\r\nSector = %d", sector); break;
			case '/': sector-=512;	rprintf("\r\nSector = %d", sector); break;
			case '\r':
			default:
				break;
			}
			// print new prompt
			rprintf("\r\ncmd>");
		}
	}

}
