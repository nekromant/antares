//*****************************************************************************
// File Name	: ds1631test.c
// 
// Title		: example usage of DS1631 library functions
// Revision		: 1.0
// Notes		:	
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
// 
// Revision History:
// When			Who			Description of change
// -----------	-----------	-----------------------
// 20-Feb-2004	pstang		Created the program
//*****************************************************************************

//----- Include Files ---------------------------------------------------------
#include <avr/io.h>		// include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>	// include interrupt support

#include "global.h"		// include our global settings
#include "uart.h"		// include uart function library
#include "rprintf.h"	// include printf function library
#include "timer.h"		// include timer function library (timing, PWM, etc)
#include "vt100.h"		// include VT100 terminal support
#include "i2c.h"		// include i2c support
#include "ds1631.h"		// include DS1631 support

void ds1631test(void);

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
	// clear terminal screen
	vt100ClearScreen();
	vt100SetCursorPos(1,1);
	// print a little intro message so we know things are working
	rprintf("\r\nWelcome to DS6131 test!\r\n");
	
	// run tests
	ds1631test();

	return 0;
}


void ds1631test(void)
{
	s16 T=0;

	// initialize i2c function library
	i2cInit();
	i2cSetBitrate(100);

	// initialize
	if(ds1631Init(DS1631_I2C_ADDR))
	{
		rprintf("DS1631 detected and initialized!\r\n");
	}
	else
	{
		rprintf("Cannot detect DS1631!\r\n");
		return;
	}
	
	// set config
	ds1631SetConfig(DS1631_I2C_ADDR, 0x0F);
	// set the temperature limit registers
	ds1631SetTH(DS1631_I2C_ADDR, 35<<8);
	ds1631SetTL(DS1631_I2C_ADDR, 30<<8);
	// read them back for verification
	rprintf("TH is set to: %d\r\n",ds1631GetTH(DS1631_I2C_ADDR)>>8);
	rprintf("TL is set to: %d\r\n",ds1631GetTL(DS1631_I2C_ADDR)>>8);

	while(1)
	{
		// start convert
		ds1631StartConvert(DS1631_I2C_ADDR);
		// wait until done
		// this works but seems to take forever (5-10 seconds)
		//while(!(ds1631GetConfig(DS1631_I2C_ADDR) & DS1631_CONFIG_DONE));
		// 12-bit conversion are only suppored to take this long
		timerPause(750);
		
		// read temp
		T = ds1631ReadTemp(DS1631_I2C_ADDR);


		// Print the raw temperature reading
		rprintf("Raw Temp: %d    ", T);
		// Print the formatted temperature reading
		rprintf("Real Temp is: ");
		rprintfNum(10, 4, TRUE , ' ', T>>8);
		rprintf(".");
		rprintfNum(10, 4, FALSE, '0', (10000*((u32)(T&0x00FF)))/256 );
		rprintf(" degrees C\r\n");

		timerPause(50);
	}
}
