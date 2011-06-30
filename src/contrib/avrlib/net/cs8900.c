/*! \file cs8900.c \brief Crystal CS8900 Ethernet Interface Driver. */
//*****************************************************************************
//
// File Name	: 'cs8900.c'
// Title		: Crystal CS8900 Ethernet Interface Driver
// Author		: Pascal Stang
// Created		: 11/7/2004
// Revised		: 11/7/2004
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
//*****************************************************************************

#include "global.h"
#include "timer.h"
#include "rprintf.h"

#include "cs8900.h"

// include configuration
#include "cs8900conf.h"

void nicInit(void)
{
	cs8900Init();
}

void nicSend(unsigned int len, unsigned char* packet)
{
	u08 timeout = 15;

	// request space in CS8900's on-chip memory for storing an outgoing frame
	cs8900Write16(CS8900_IO_TXCMD, TX_START_ALL_BYTES);
	cs8900Write16(CS8900_IO_TXLENGTH, len);
	// check if CS8900 is ready to accept the frame we want to send
	// (timeout after 1.5ms since it should only take 1.25ms to
	//  finish sending previous frame.  If we timeout, it's probably
	//  because there's no link, no ethernet cable.)
	while(!(cs8900ReadReg(PP_BusST) & READY_FOR_TX_NOW) && timeout)
	{
		// wait 100us
		delay_us(100);
		timeout--;
	}
	// write packet data bytes
	cs8900CopyToFrame(packet, len);

	// packet is automatically sent upon completion of above write
}

unsigned int nicPoll(unsigned int maxlen, unsigned char* packet)
{
	unsigned int packetLength;

	packetLength = cs8900BeginPacketRetreive();

	// if there's no packet or an error - exit without ending the operation
	if( !packetLength )
		return 0;

	// drop anything too big for the buffer
	if( packetLength > maxlen )
	{
		cs8900EndPacketRetreive();
		return 0;
	}
	
	// copy the packet data into the packet buffer
	cs8900RetreivePacketData( packet, packetLength );
	cs8900EndPacketRetreive();
		
	return packetLength;
}

void nicGetMacAddress(u08* macaddr)
{
	// read MAC address registers
	// TODO: check byte order here!
	*((unsigned short*)(macaddr+0)) = cs8900ReadReg(PP_IA+0);
	*((unsigned short*)(macaddr+2)) = cs8900ReadReg(PP_IA+2);
	*((unsigned short*)(macaddr+4)) = cs8900ReadReg(PP_IA+4);
}

void nicSetMacAddress(u08* macaddr)
{
	// write MAC address registers
	cs8900WriteReg(PP_IA+0, (macaddr[1]<<8) + macaddr[0] );
	cs8900WriteReg(PP_IA+2, (macaddr[3]<<8) + macaddr[2] );
	cs8900WriteReg(PP_IA+4, (macaddr[5]<<8) + macaddr[4] );
}

unsigned int cs8900BeginPacketRetreive(void)
{
	unsigned short status;

	// check RxEvent
	status = cs8900ReadReg(PP_RxEvent);

	if( !((status&RX_OK)||(status&RX_IA)||(status&RX_BROADCAST)) )
	{
		return 0;
	}

//	return cs8900ReadReg(PP_RxFrameByteCnt);
	// read RxStatus high-byte first
	status =  cs8900Read(CS8900_IO_RXTX_DATA_PORT0+1)<<8;
	status |= cs8900Read(CS8900_IO_RXTX_DATA_PORT0+0);
	// read packet length high-byte first
	status =  cs8900Read(CS8900_IO_RXTX_DATA_PORT0+1)<<8;
	status |= cs8900Read(CS8900_IO_RXTX_DATA_PORT0+0);

	return status;
}

void cs8900RetreivePacketData(u08* packet, unsigned int packetLength )
{
	cs8900CopyFromFrame(packet, packetLength);
}

void cs8900EndPacketRetreive(void)
{
	// dummy read first four bytes
	//cs8900CopyFromFrame(packet, 4);
}



void cs8900InitPorts(void)
{
#if MEMORY_MAPPED_NIC == 1
  	// enable external SRAM interface - no wait states
  	sbi(MCUSR, SRE);
#else
	// set address port to output
	outb(CS8900_ADDRESS_DDR, CS8900_ADDRESS_MASK);
    
	// set data port to input with pull-ups
	outb(CS8900_DATA_DDR, 0x00);
	outb(CS8900_DATA_PORT, 0xFF);

	// initialize the control port read and write pins to de-asserted
	sbi( CS8900_CONTROL_PORT, CS8900_CONTROL_READPIN );
	sbi( CS8900_CONTROL_PORT, CS8900_CONTROL_WRITEPIN );
	// set the read and write pins to output
	sbi( CS8900_CONTROL_DDR, CS8900_CONTROL_READPIN );
	sbi( CS8900_CONTROL_DDR, CS8900_CONTROL_WRITEPIN );
#endif
	// set reset pin to output
	sbi( CS8900_RESET_DDR, CS8900_RESET_PIN );
}

void cs8900Init(void)
{
	cs8900InitPorts();

	// assert hardware reset
	sbi( CS8900_RESET_PORT, CS8900_RESET_PIN );
	// wait
	delay_ms(10);
	// release hardware reset
	cbi( CS8900_RESET_PORT, CS8900_RESET_PIN );
	delay_ms(10);
	
	// Reset the Ethernet-Controller
	cs8900Write16(CS8900_IO_PP_PTR, PP_SelfCTL);
	cs8900Write16(CS8900_IO_PP_DATA_PORT0, POWER_ON_RESET);
	// wait until chip-reset is done
	cs8900Write16(CS8900_IO_PP_PTR, PP_SelfST);
	while(!(cs8900Read16(CS8900_IO_PP_DATA_PORT0) & INIT_DONE));

	// set our MAC as Individual Address
	cs8900WriteReg(PP_IA+0, (CS8900_MAC1<<8) + CS8900_MAC0 );
	cs8900WriteReg(PP_IA+2, (CS8900_MAC3<<8) + CS8900_MAC2 );
	cs8900WriteReg(PP_IA+4, (CS8900_MAC5<<8) + CS8900_MAC4 );
	// configure the Physical Interface
	cs8900WriteReg(PP_LineCTL, SERIAL_RX_ON | SERIAL_TX_ON);
	cs8900WriteReg(PP_RxCTL, RX_OK_ACCEPT | RX_IA_ACCEPT | RX_BROADCAST_ACCEPT );
}

void cs8900Write(unsigned char address, unsigned char data)
{
	// assert the address
	outb(CS8900_ADDRESS_PORT, address | (inb(CS8900_ADDRESS_PORT)&~CS8900_ADDRESS_MASK));
	// set data bus as output
	outb(CS8900_DATA_DDR, 0xFF);
	// place data on bus
	outb(CS8900_DATA_PORT, data);
	// clock write pin
	cbi(CS8900_CONTROL_PORT, CS8900_CONTROL_WRITEPIN);
	nop();
	nop();
	nop();
	nop();
	sbi(CS8900_CONTROL_PORT, CS8900_CONTROL_WRITEPIN);
	// set data bus back to input with pullups enabled
	outb(CS8900_DATA_DDR, 0x00);
	outb(CS8900_DATA_PORT, 0xFF);
}

unsigned char cs8900Read(unsigned char address)
{
	unsigned char data;
	// assert the address
	outb(CS8900_ADDRESS_PORT, address | (inb(CS8900_ADDRESS_PORT)&~CS8900_ADDRESS_MASK));
	// set data bus to input with pullups enabled
	outb(CS8900_DATA_DDR, 0x00);
	outb(CS8900_DATA_PORT, 0xFF);
	// assert read
	cbi(CS8900_CONTROL_PORT, CS8900_CONTROL_READPIN);
	nop();
	nop();
	nop();
	nop();
	// read in the data
	data = inb( CS8900_DATA_PIN );
	// negate read
	sbi(CS8900_CONTROL_PORT, CS8900_CONTROL_READPIN);
	// return data
	return data;
}

void cs8900Write16(unsigned char address, unsigned short data)
{
	cs8900Write(address+0, data);	
	cs8900Write(address+1, data>>8);	
}

unsigned short cs8900Read16(unsigned char address)
{
	unsigned short data;
	data =  cs8900Read(address+0);
	data |= cs8900Read(address+1)<<8;
	return data;
}

// writes a word in little-endian byte order to a specified PacketPage address
void cs8900WriteReg(unsigned short address, unsigned short data)
{
	cs8900Write16(CS8900_IO_PP_PTR, address);
	cs8900Write16(CS8900_IO_PP_DATA_PORT0, data);
}

// reads a word in little-endian byte order from a specified PacketPage address
unsigned short cs8900ReadReg(unsigned short address)
{
	cs8900Write16(CS8900_IO_PP_PTR, address);
	return cs8900Read16(CS8900_IO_PP_DATA_PORT0);
}

// copies bytes from MCU-memory to frame port
// NOTES: * an odd number of byte may only be transfered
//          if the frame is written to the end!
//        * MCU-memory MUST start at word-boundary

void cs8900CopyToFrame(unsigned char *source, unsigned short size)
{
	while(size>1)
	{
		cs8900Write16(CS8900_IO_RXTX_DATA_PORT0, *((unsigned short*)source));
		source += 2;
		size -= 2;
	}
	// if odd num. of bytes...
	// write leftover byte (the LAN-controller ignores the highbyte)
	if(size)
		cs8900Write16(CS8900_IO_RXTX_DATA_PORT0, *(unsigned char*)source);
}

// copies bytes from frame port to MCU-memory
// NOTES: * an odd number of byte may only be transfered
//          if the frame is read to the end!
//        * MCU-memory MUST start at word-boundary

void cs8900CopyFromFrame(unsigned char *dest, unsigned short size)
{
	while(size>1)
	{
		*((unsigned short *)dest) = cs8900Read16(CS8900_IO_RXTX_DATA_PORT0);
		dest += 2;
		size -= 2;
	}
  
	// check for leftover byte...
	// the LAN-Controller will return 0 for the highbyte
	if(size)	
		*(unsigned char *)dest = cs8900Read16(CS8900_IO_RXTX_DATA_PORT0);
}

void cs8900IORegDump(void)
{
	rprintfProgStrM("CS8900 I/O Registers\r\n");
	rprintfProgStrM(" FRAME   ISQ  ADDR DATA0 DATA1\r\n");
	rprintfProgStrM("-------------------------------\r\n");
	rprintfProgStrM("  ");
	rprintfu16(cs8900Read16(CS8900_IO_RXTX_DATA_PORT0));
	rprintfProgStrM("  ");
	rprintfu16(cs8900Read16(CS8900_IO_ISQ));
	rprintfProgStrM("  ");
	rprintfu16(cs8900Read16(CS8900_IO_PP_PTR));
	rprintfProgStrM("  ");
	rprintfu16(cs8900Read16(CS8900_IO_PP_DATA_PORT0));
	rprintfProgStrM("  ");
	rprintfu16(cs8900Read16(CS8900_IO_PP_DATA_PORT1));
	rprintfCRLF();
}

void cs8900RegDump(void)
{
	rprintfProgStrM("CS8900 PacketPage Registers\r\n");
	rprintfProgStrM("CHIP ID: ");
	rprintfu16(cs8900ReadReg(PP_ChipID));
	rprintfCRLF();

	rprintfProgStrM("PP_ISAIOB: ");
	rprintfu16(cs8900ReadReg(PP_ISAIOB));
	rprintfCRLF();

	rprintfProgStrM("MAC addr: ");
	rprintfu16(cs8900ReadReg(PP_IA+0));
	rprintfu16(cs8900ReadReg(PP_IA+2));
	rprintfu16(cs8900ReadReg(PP_IA+4));
	rprintfCRLF();
}

void nicRegDump(void)
{
	cs8900IORegDump();
	cs8900RegDump();
}


u08 cs8900LinkStatus(void)
{
	if(cs8900ReadReg(PP_LineST) & LINK_OK)
		return 1;
	else
		return 0;
}
