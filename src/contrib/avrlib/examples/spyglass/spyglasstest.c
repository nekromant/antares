//*****************************************************************************
// File Name	: spyglasstest.c
// 
// Title		: example usage of the Spyglass I2C-controller user interface
// Revision		: 1.0
// Notes		:	
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
// 
// Revision History:
// When			Who			Description of change
// -----------	-----------	-----------------------
// 28-Aug-2005	pstang		Created the program
//*****************************************************************************

#include <avr/io.h>

#include "global.h"
#include "timer.h"
#include "uart.h"
#include "rprintf.h"
#include "vt100.h"
#include "lcd.h"
#include "debug.h"
#include "i2c.h"
#include "spyglass.h"

// defines
#define TIMER_PRESCALE		1024
#define TIMER_INTERVAL		(F_CPU/TIMER_PRESCALE/100)

// global variables
static volatile unsigned long UptimeMs;
u08 Contrast=0;

// prototypes
void spyglassTest(void);
void systickHandler(void);
void i2cDeviceSearch(void);
void serviceLocal(void);

int main(void)
{
	timerInit();				// initializing timers
	uartInit();					// initializing serial port
	uartSetBaudRate(115200);	// set serial port baud rate
	rprintfInit(uartSendByte);	// direct rprintf() output to go to serial port
	timerPause(100);			// wait for a moment

	// print welcome message
	rprintf("\r\n\n\nWelcome to the Spyglass UI test.\r\n");

	// begin test application
	spyglassTest();

	return 0;
}

void spyglassTest(void)
{
	// initializing Spyglass interface and I2C bus
	rprintf("Initializing Spyglass communication...");
	spyglassInit();
	spyglassLcdInit();
	rprintf("Done!\r\n");
	rprintf("Printing 'Hello World!' message to spyglass LCD.\r\n");

	rprintfInit(spyglassLcdWriteChar);
	spyglassLcdGotoXY(0,0);
	rprintfProgStrM("Hello World!");
	rprintfInit(uartSendByte);

	timerPause(1000);

	// initialize systick timer
	rprintf("Initializing Periodic Timer\r\n");
	timer2SetPrescaler(TIMERRTC_CLK_DIV1024);
	// attach the 'systickHandler' so that it gets called every time Timer2 overflows
	timerAttach(TIMER2OVERFLOW_INT, systickHandler);

	rprintf("Starting local command prompt.  Type '?' to get help.\r\n");
	rprintf("cmd>");

	while(1)
	{
		serviceLocal();
	}
}

void systickHandler(void)
{
	u16 tms;
	u08 ts,tm,th;
	u08 pb;

	// set timer for 10ms interval
	TCNT2 = (unsigned char)-TIMER_INTERVAL;
	// count up on uptime
	UptimeMs += 10;

	// if we at a 100ths millisecond interval, update the display
	if(!(UptimeMs % 100))
	{
		// redirect rprintf() output to spyglass LCD
		rprintfInit(spyglassLcdWriteChar);
		
		// print banner message
		spyglassLcdGotoXY(0,0);
		rprintfProgStrM("**** SpyglassUI ****");

		// read pushbuttons and take appropriate action
		pb = spyglassGetPushbuttons();
		spyglassLcdGotoXY(0,1);
		rprintf("Buttons:    ");
		rprintfNum(2,8,FALSE,'0',pb);

		if((pb & 0x01) && (Contrast < 255))
			Contrast++;
		if((pb & 0x02) && (Contrast > 0))
			Contrast--;
		if(pb & 0x08)
			spyglassSetLeds(0x01);
		if(pb & 0x10)
			spyglassSetLeds(0x02);
		
		// show display contrast
		spyglassLcdGotoXY(0,2);
		rprintf("LCD Contrast:    ");
		rprintfNum(10,3,FALSE,' ',Contrast);
		spyglassSetLcdContrast(Contrast);

		// show time
		tms = (UptimeMs)%1000;
		ts  = (UptimeMs/1000)%60;
		tm  = (UptimeMs/60000l)%60;
		th  = (UptimeMs/3600000l);
		spyglassLcdGotoXY(0,3);
		rprintf("Time:");
		rprintfNum(10,3,FALSE,' ',th);
		rprintfChar(':');
		rprintfNum(10,2,FALSE,'0',tm);
		rprintfChar(':');
		rprintfNum(10,2,FALSE,'0',ts);
		rprintfChar('.');
		rprintfNum(10,3,FALSE,'0',tms);
		rprintf("ms");
		
		// return rprintf() output to serial port
		rprintfInit(uartSendByte);
	}
}

void serviceLocal(void)
{
	int c;

	// a little command-prompt utility to play with the spyglass UI
	// all commands are single characters

	if( (c = uartGetByte()) != -1)
	{
		// echo command to terminal
		uartSendByte(c);
		// process command
		switch(c)
		{
		case 'i': spyglassLcdInit(); break;
		case 'h':
			rprintfInit(spyglassLcdWriteChar);
			spyglassLcdGotoXY(0,0);
			rprintf("*** HELLO WORLD  ***");
			rprintfInit(uartSendByte);
			break;
		case 'p':
			rprintf("Pushbutton State: 0x%x\r\n", spyglassGetPushbuttons());
			break;
		case '+': if(Contrast<255) Contrast++; rprintf("\r\nLCD Contrast: %d\r\n", Contrast); spyglassSetLcdContrast(Contrast); break;
		case '-': if(Contrast>0)   Contrast--; rprintf("\r\nLCD Contrast: %d\r\n", Contrast); spyglassSetLcdContrast(Contrast); break;
		case 'l': spyglassSetLeds(0x00); break;
		case 'L': spyglassSetLeds(0xFF); break;
		case 'b': spyglassSetBeeper(0); break;
		case 'B': spyglassSetBeeper(1); break;
		case 'x':
			i2cDeviceSearch();
			break;
		case '?':
			rprintfProgStrM("\r\n");
			rprintfProgStrM("--- Spyglass Commands: ---\r\n");
			rprintfProgStrM("(i) Initialize Spyglass LCD\r\n");
			rprintfProgStrM("(h) Print 'Hello World' message to Spyglass LCD\r\n");
			rprintfProgStrM("(p) Get Spyglass pushbutton state\r\n");
			rprintfProgStrM("(+) Increase contrast number (lightens contrast)\r\n");
			rprintfProgStrM("(-) Decrease contrast number (darkens contrast)\r\n");
			rprintfProgStrM("(l) Set Spyglass User LEDs to OFF\r\n");
			rprintfProgStrM("(L) Set Spyglass User LEDs to ON\r\n");
			rprintfProgStrM("(b) Set Spyglass beeper to OFF\r\n");
			rprintfProgStrM("(B) Set Spyglass beeper to ON\r\n");
			rprintfProgStrM("--- General Commands: ---\r\n");
			rprintfProgStrM("(x) Search for I2C devices on the bus\r\n");
			rprintfProgStrM("(?) Help\r\n");
			break;
		case '\r':
		default:
			break;
		}
		// print new prompt
		rprintfProgStrM("\r\ncmd>");
	}
}

void i2cDeviceSearch(void)
{
	u08 i2cAddr;
	u08 i2cStat;

	// this function searches all device addresses on the I2C bus
	// and returns addresses that are live (have a device)

	rprintf("\r\nSearching for I2c devices on bus\r\n");
	
	for(i2cAddr = 0; i2cAddr<0x80; i2cAddr+=2)
	{
		i2cStat = i2cMasterSendNI(i2cAddr, 0, 0);
		if(i2cStat == I2C_OK)
			rprintf("Device present at address 0x%x\r\n", i2cAddr);
	}
	rprintf("Search complete.\r\n");
}
