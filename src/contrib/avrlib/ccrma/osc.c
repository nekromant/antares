/*! \file osc.c \brief Open Sound Control (OSC) client functions. */
//*****************************************************************************
//
// File Name	: 'osc.c'
// Title		: Open Sound Control (OSC) client functions
// Author		: Pascal Stang - Copyright (C) 2002
// Created		: 10/30/2002
// Revised		: 11/4/2002
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// Description	: This code implements a subset of the OSC protocol and
//		messages.  It is meant to be used with the OSC extension for the visual
//		programming and data-processing package Pure-Data (or PD).  Note that
//		this code sends OSC messages over serial RS-232, not a network.  You
//		must use these functions with a suitable OSC SERIAL server (receiver)
//		on a host machine.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "global.h"
#include "buffer.h"
#include "uart.h"
#include "osc.h"

// global variables
// ready flag from uart 
extern volatile u08 uartReadyTx;

// functions
void oscInit(void)
{
	// initialize uart (if not already done)
	uartInit();
	// set the default communication rate
	uartSetBaudRate(38400);
}

void oscSendMessage(char *address)
{
	// write OSC packet header, argument length = 0bytes
	oscWriteHeader(address, 0);
	// write OSC address to packet
	oscWriteString(address);
	// apply checksum
	oscWriteChecksum();
	// send buffer
	uartSendTxBuffer();
	// wait for completion, transmitter to be ready
	while(!uartReadyTx);
}

void oscSendMessageInt(char *address, u32 arg)
{
	// write OSC packet header, argument length = 4bytes
	oscWriteHeader(address, 4);
	// write OSC address to packet
	oscWriteString(address);
	// copy arg to buffer
	bufferAddToEnd(uartGetTxBuffer(), *(((unsigned char*)(&arg))+3) );
	bufferAddToEnd(uartGetTxBuffer(), *(((unsigned char*)(&arg))+2) );
	bufferAddToEnd(uartGetTxBuffer(), *(((unsigned char*)(&arg))+1) );
	bufferAddToEnd(uartGetTxBuffer(), *(((unsigned char*)(&arg))+0) );
	// apply checksum
	oscWriteChecksum();
	// send buffer
	uartSendTxBuffer();
	// wait for completion, transmitter to be ready
	while(!uartReadyTx);
}

void oscSendMessageIntInt(char *address, u32 arg1, u32 arg2)
{
	// write OSC packet header, argument length = 8bytes
	oscWriteHeader(address, 8);
	// write OSC address to packet
	oscWriteString(address);
	// copy arg1 to buffer
	bufferAddToEnd(uartGetTxBuffer(), *(((unsigned char*)(&arg1))+3) );
	bufferAddToEnd(uartGetTxBuffer(), *(((unsigned char*)(&arg1))+2) );
	bufferAddToEnd(uartGetTxBuffer(), *(((unsigned char*)(&arg1))+1) );
	bufferAddToEnd(uartGetTxBuffer(), *(((unsigned char*)(&arg1))+0) );
	// copy arg2 to buffer
	bufferAddToEnd(uartGetTxBuffer(), *(((unsigned char*)(&arg2))+3) );
	bufferAddToEnd(uartGetTxBuffer(), *(((unsigned char*)(&arg2))+2) );
	bufferAddToEnd(uartGetTxBuffer(), *(((unsigned char*)(&arg2))+1) );
	bufferAddToEnd(uartGetTxBuffer(), *(((unsigned char*)(&arg2))+0) );
	// apply checksum
	oscWriteChecksum();
	// send buffer
	uartSendTxBuffer();
	// wait for completion, transmitter to be ready
	while(!uartReadyTx);
}

void oscSendMessageString(char *address, char *arg)
{
	u08 len;

	// calculate length of argument string
	for(len=0; PRG_RDB(arg+len); len++);
	len++;						// count a minumum of one null for termination
	if(len&0x03)				// are pad bytes necessary?
		len += 4-(len&0x03);	// add null pad bytes to reach multiple of four

	// write OSC packet header, argument length = 8bytes
	oscWriteHeader(address, len);
	// write OSC address to packet
	oscWriteString(address);
	// write string buffer
	oscWriteString(arg);
	// apply checksum
	oscWriteChecksum();
	// send buffer
	uartSendTxBuffer();
	// wait for completion, transmitter to be ready
	while(!uartReadyTx);
}

void oscWriteHeader(char *address, u08 arglen)
{
	u08 len;
	// write OSC packet header
	bufferAddToEnd(uartGetTxBuffer(), OSC_HEADER);
	// determine padded length of address
	for(len=0; PRG_RDB(address+len); len++);
	len++;						// count a minumum of one null for termination
	if(len&0x03)				// are pad bytes necessary?
		len += 4-(len&0x03);	// add null pad bytes to reach multiple of four
	// write length to packet header
	bufferAddToEnd(uartGetTxBuffer(), len+arglen);
}

void oscWriteString(char *string)
{
	u08 temp=1;
	u08 len=0;

	// write OSC string to packet
	// copy string's null-termination intentionally
	while(temp)
	{
		temp = PRG_RDB(string++);
		bufferAddToEnd(uartGetTxBuffer(), temp);
		len++;
	}

	// pad the string as necessary to reach a 4-byte multiple
	// Note: (len&0x03) == (len%4)
	//for(; (len&0x03); len++)
	while(len&0x03)
	{
		bufferAddToEnd(uartGetTxBuffer(), 0);
		len++;
	}
}

void oscWriteChecksum(void)
{
	u08 i;
	u08 chksum = 0;
	// calculate checksum
	for(i=2; i<uartGetTxBuffer()->datalength; i++)
		chksum += bufferGetAtIndex(uartGetTxBuffer(), i);
	// write checksum to packet
	bufferAddToEnd(uartGetTxBuffer(), chksum);
}
