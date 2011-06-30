//*****************************************************************************
// File Name	: stxetxtest.c
// Title		: Example usage of STX/ETX packet protocol and library code
// Revision		: 0.1
// Notes		: 
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
// 
// Revision History:
// When			Who			Description of change
// -----------	-----------	-----------------------
// 20-Nov-2002	pstang		Created the program
//*****************************************************************************

//----- Include Files ---------------------------------------------------------
#include <avr/io.h>		// include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>	// include interrupt support
#include <avr/pgmspace.h>

#include "global.h"		// include our global settings
#include "uart.h"		// include uart function library
#include "rprintf.h"	// include printf function library
#include "timer.h"		// include timer function library
#include "lcd.h"		// include LCD support
#include "stxetx.h"		// include STX/ETX packet support


// make a transmitting packet buffer area in memory
// where we can assemble the data we wish to transmit
// using stx/etx
unsigned char TxPacket[20];

// function prototypes
void receiveStxEtx(void);
void transmitStxEtx(void);
u08 getSw(void);

// main code
int main(void)
{
	// initialize the AVRlib libraries
	timerInit(); 				// initialize the timer system
	uartInit(); 				// initialize the UART (serial port)
	lcdInit();
	rprintfInit(lcdDataWrite);	// init rprintf

	// we will be using the AVR UART to communicate the
	// STX/ETX protocol to another AVR board or to a PC
	
	// initialize stxetx to use the UART for sending data
	stxetxInit(uartSendByte);

	// set baud rate to 4800 (less than 5000) if communicating using
	// the Linx LC radio modules
	uartSetBaudRate(4800);

	// choose to either transmit or receive
	transmitStxEtx();
	//receiveStxEtx();

	return 0;
}

void transmitStxEtx(void)
{
	u16 packetNum=0;

	lcdClear();

	while(1)
	{
		// assemble the data I wish to send inside a STX/ETX packet
		// send the string "ALOHA"
		TxPacket[0] = 'A';
		TxPacket[1] = 'L';
		TxPacket[2] = 'O';
		TxPacket[3] = 'H';
		TxPacket[4] = 'A';
		// send the packet number
		TxPacket[5] = packetNum>>8;		// high byte
		TxPacket[6] = packetNum;		// low byte
		// send the current state of PORTA
		TxPacket[7] = inb(PINA);

		// send the packet with:
		// packet status = 0
		// packet type = 0x55
		// (the packet type and status may be, and mean, anything the user wishes)
		// 8-bytes of user data
		stxetxSend(0, 0x55, 8, TxPacket);

		// output our packet data to the LCD
		lcdGotoXY(0,0);		rprintf("Sending..."); 
		lcdGotoXY(20,0);	rprintf("PORTA: "); rprintfu08(inb(PINA));
		lcdGotoXY(0,1);		rprintf("String: ALOHA");
		lcdGotoXY(20,1);	rprintf("PacketNum: "); rprintfu16(packetNum);

		// increment our packet number
		packetNum++;
		// pause for a moment
		timerPause(10);
	}
}

void receiveStxEtx(void)
{
	u16 packetNum=0;
	u08* dataPtr;

	lcdClear();

	while(1)
	{
		lcdGotoXY(0,0);		rprintf("Receiving..."); 

		// here we get the UART's receive buffer and give it to the STX/ETX
		// packet processing function.  If the packet processor finds a valid
		// packet in the buffer, it will return true.
		if(stxetxProcess(uartGetRxBuffer()))
		{
			// sxtetxProcess has reported that it found a packet
			// let's get the data...

			// (NOTE: although I discard the status, type, and datalength
			// below, it would be important if I were sending more than one
			// kind of packet)

			// get the packet's status
			stxetxGetRxPacketStatus();
			// get the packet's type
			stxetxGetRxPacketType();
			// get the packet's datalength
			stxetxGetRxPacketDatalength();
			// get a pointer to the place where the received data is stored
			dataPtr = stxetxGetRxPacketData();

			// decode packet number
			packetNum = (dataPtr[5]<<8) | dataPtr[6];

			// output our packet data to the LCD
			lcdGotoXY(20,0);	rprintf("PORTA: "); rprintfu08( dataPtr[7] );
			lcdGotoXY(0,1);		rprintf("String: ");
			// print out the string
			rprintfChar( dataPtr[0] );
			rprintfChar( dataPtr[1] );
			rprintfChar( dataPtr[2] );
			rprintfChar( dataPtr[3] );
			rprintfChar( dataPtr[4] );
			lcdGotoXY(20,1);	rprintf("PacketNum: "); rprintfu16(packetNum);
		}
	}
}
