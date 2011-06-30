//*****************************************************************************
// File Name	: i2ctest.c
// 
// Title		: example usage of I2C library functions
// Revision		: 1.0
// Notes		:	
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
// 
// Revision History:
// When			Who			Description of change
// -----------	-----------	-----------------------
// 03-Feb-2003	pstang		Created the program
//*****************************************************************************

 
//----- Include Files ---------------------------------------------------------
#include <avr/io.h>		// include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>	// include interrupt support

#include "global.h"		// include our global settings
#include "uart.h"		// include uart function library
#include "rprintf.h"	// include printf function library
#include "a2d.h"		// include A/D converter function library
#include "timer.h"		// include timer function library (timing, PWM, etc)
#include "vt100.h"		// include VT100 terminal support
#include "i2c.h"		// include i2c support
#include "debug.h"		// include debug functions


#define LOCAL_ADDR	0xA0
#define TARGET_ADDR	0xA0

// local data buffer
unsigned char localBuffer[] = "Pascal is cool!!Pascal is Cool!!";
unsigned char localBufferLength = 0x20;

void i2cTest(void);
void i2cSlaveReceiveService(u08 receiveDataLength, u08* receiveData);
u08 i2cSlaveTransmitService(u08 transmitDataLengthMax, u08* transmitData);
void i2cMasterSendDiag(u08 deviceAddr, u08 length, u08* data);
void testI2cMemory(void);
void showByte(u08 byte);

//----- Begin Code ------------------------------------------------------------
int main(void)
{
	// initialize our libraries
	// initialize the UART (serial port)
	uartInit();
	// make all rprintf statements use uart for output
	rprintfInit(uartSendByte);
	// initialize the timer system
	timerInit();
	// clear terminal screen
	vt100ClearScreen();
	vt100SetCursorPos(1,1);
	// print a little intro message so we know things are working
	rprintf("\r\nWelcome to i2c test!\r\n");
	
	// run tests
	
	i2cTest();

	return 0;
}


void i2cTest(void)
{
	u08 c=0;
	showByte(0x55);

	// initialize i2c function library
	i2cInit();
	// set local device address and allow response to general call
	i2cSetLocalDeviceAddr(LOCAL_ADDR, TRUE);
	// set the Slave Receive Handler function
	// (this function will run whenever a master somewhere else on the bus
	//  writes data to us as a slave)
	i2cSetSlaveReceiveHandler( i2cSlaveReceiveService );
	// set the Slave Transmit Handler function
	// (this function will run whenever a master somewhere else on the bus
	//  attempts to read data from us as a slave)
	i2cSetSlaveTransmitHandler( i2cSlaveTransmitService );

	timerPause(500);
	showByte(0xAA);

	while(1)
	{
		rprintf("Command>");
		while(!c) uartReceiveByte(&c);
		
		switch(c)
		{
		case 's':
			rprintf("Send: ");
			// get string from terminal
			localBufferLength = 0;
			c = 0;
			while((c != 0x0D) && (localBufferLength < 0x20))
			{
				while(!uartReceiveByte(&c));
				localBuffer[localBufferLength++] = c;
				uartSendByte(c);
			}
			// switch CR to NULL to terminate string
			localBuffer[localBufferLength-1] = 0;
			// send string over I2C
			i2cMasterSend(TARGET_ADDR, localBufferLength, localBuffer);
			//i2cMasterSendNI(TARGET_ADDR, localBufferLength, localBuffer);
			rprintfCRLF();
			break;
		case 'r':
			rprintf("Receive: ");
			// receive string over I2C
			i2cMasterReceive(TARGET_ADDR, 0x10, localBuffer);
			//i2cMasterReceiveNI(TARGET_ADDR, 0x10, localBuffer);
			// format buffer
			localBuffer[0x10] = 0;
			rprintfStr(localBuffer);
			rprintfCRLF();
			break;
		case 'm':
			testI2cMemory();
			break;
		default:
			rprintf("Unknown Command!");
			break;
		}
		c = 0;
		rprintfCRLF();
	}

}


// slave operations
void i2cSlaveReceiveService(u08 receiveDataLength, u08* receiveData)
{
	u08 i;

	// this function will run when a master somewhere else on the bus
	// addresses us and wishes to write data to us

	showByte(*receiveData);
	cbi(PORTB, PB6);

	// copy the received data to a local buffer
	for(i=0; i<receiveDataLength; i++)
	{
		localBuffer[i] = *receiveData++;
	}
	localBufferLength = receiveDataLength;
}

u08 i2cSlaveTransmitService(u08 transmitDataLengthMax, u08* transmitData)
{
	u08 i;

	// this function will run when a master somewhere else on the bus
	// addresses us and wishes to read data from us

	showByte(*transmitData);
	cbi(PORTB, PB7);

	// copy the local buffer to the transmit buffer
	for(i=0; i<localBufferLength; i++)
	{
		*transmitData++ = localBuffer[i];
	}

	localBuffer[0]++;

	return localBufferLength;
}

void i2cMasterSendDiag(u08 deviceAddr, u08 length, u08* data)
{
	// this function is equivalent to the i2cMasterSendNI() in the I2C library
	// except it will print information about transmission progress to the terminal

	// disable TWI interrupt
	cbi(TWCR, TWIE);

	// send start condition
	i2cSendStart();
	i2cWaitForComplete();
	rprintf("STA-");

	// send device address with write
	i2cSendByte( deviceAddr&0xFE );
	i2cWaitForComplete();
	rprintf("SLA+W-");
	
	// send data
	while(length)
	{
		i2cSendByte( *data++ );
		i2cWaitForComplete();
		rprintf("DATA-");
		length--;
	}
	
	// transmit stop condition
	// leave with TWEA on for slave receiving
	i2cSendStop();
	while( !(inb(TWCR) & BV(TWSTO)) );
	rprintf("STO");

	// enable TWI interrupt
	sbi(TWCR, TWIE);
}

void testI2cMemory(void)
{
	u08 i;
	u08 txdata[66];
	u08 rxdata[66];

	rprintfProgStrM("\r\nRunning I2C memory test (24xxyy devices)\r\n");

	// compose address
	txdata[0] = 0;
	txdata[1] = 0;
	// compose data
	for(i=0; i<16; i++)
		txdata[2+i] = localBuffer[i];
	// send address and data
	i2cMasterSendNI(TARGET_ADDR, 18, txdata);
	
	rprintfProgStrM("Stored data: ");
	// null-terminate data
	txdata[18] = 0;
	rprintfStr(&txdata[2]);
	rprintfCRLF();
	
	timerPause(100);

	// send address
	i2cMasterSendNI(TARGET_ADDR, 2, txdata);
	// get data
	i2cMasterReceiveNI(TARGET_ADDR, 16, rxdata);
	// null-terminate received string
	rxdata[16] = 0;

	rprintfProgStrM("Received data: ");
	rprintfStr(rxdata);
	rprintfCRLF();
/*
	u08 c;
	u16 addr=0;

	while(1)
	{
		while(!uartReceiveByte(&c));

		switch(c)
		{
		case '+':
			addr+=64;
			break;
		case '-':
			addr-=64;
			break;
		}
		c = 0;
		txdata[0] = (addr>>8);
		txdata[1] = (addr&0x00FF);
		i2cMasterSendNI(TARGET_ADDR, 2, txdata);
		i2cMasterReceiveNI(TARGET_ADDR, 64, rxdata);
		rprintfProgStrM("Received data at ");
		rprintfu16(addr);
		rprintfProgStrM(":\r\n");
		debugPrintHexTable(64, rxdata);
	}
*/
}

void showByte(u08 byte)
{
	// set PORTB to output
	outb(DDRB, 0xFF);
	// output byte to LEDs
	outb(PORTB, ~byte);
}
