/*! \file prism2.c \brief Prism2 802.11b Wireless-LAN Interface Driver. */
//*****************************************************************************
//
// File Name	: 'prism2.c'
// Title		: Prism2 802.11b Wireless-LAN Interface Driver
// Author		: Pascal Stang
// Created		: 12/27/2004
// Revised		: 1/7/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// Description	: This driver provides initialization and transmit/receive
//		functions for the Prism2 802.11b Wireless-LAN Controller.
//
//*****************************************************************************

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include "global.h"
#include "timer.h"
#include "rprintf.h"
#include "debug.h"

#include "net.h"
#include "prism2.h"

// include configuration
#include "prism2conf.h"

u16 TxHeader[34];

void nicInit(void)
{
	prism2Init();
}

void nicSend(unsigned int len, unsigned char* packet)
{
	u16 i;
	u16 txfid;
	u08 stat;
	// request free buffer space to store outgoing frame
	prism2Command(PRISM2_CMD_ALLOC, len+44+14+6);
	// wait for buffer to be allocated
	while( !(prism2Read16(PRISM2_REG_EVSTAT) & PRISM2_EVENT_ALLOC) );
	// get the buffer FID
	txfid = prism2Read16(PRISM2_REG_ALLOCFID);
	// ACK the alloc event
	prism2Write16(PRISM2_REG_EVACK, PRISM2_EVENT_ALLOC);

//	rprintf("PRISM2: TxFID=0x");
//	rprintfu16(txfid);
//	rprintfCRLF();

	// adjust packet length because MAC addresses and type
	// will be written seperately from packet payload
	len-=14;

	// write the outgoing frame to BAP
	// begin with control structure
	prism2SetupTxHeader(TxHeader);
	
	// write dest and src MAC addresses
	for(i=0;i<6;++i)
		TxHeader[23+i] = packet[i*2+1]<<8 | packet[i*2];
	// write length
	TxHeader[29] = htons(len+8);
	// write type
	TxHeader[33] = packet[13]<<8 | packet[12];

//	debugPrintHexTable(34*2, (u08*)TxHeader);
//	rprintfCRLF();
//	debugPrintHexTable(len, &packet[14]);

	// write Tx header out to BAP
	prism2WriteBAP0(txfid, 0, TxHeader, 34);
	// write packet out to BAP
	prism2WriteBAP0(txfid, 68, (u16*)&packet[14], (len+1)>>1);
	// issue transmit command	
	stat = prism2Command(PRISM2_CMD_TX, txfid);
	if(stat)
		rprintf("Transmit failed: 0x%x\r\n", stat);
	// do cleanup
	prism2EventCheck();
}

void nicGetMacAddress(u08* macaddr)
{
	prism2GetMacAddress(macaddr);
}

void nicSetMacAddress(u08* macaddr)
{
	// not yet supported
}

void nicRegDump(void)
{
	prism2CardRegDump();
	prism2RegDump();
}

void prism2SetupTxHeader(u16* header)
{
	u16 i;
    
	// clear out header
	for(i=0;i<22;i++)
		header[i] = 0x00;

	// set TxRate and retry count
	header[5] = (0<<8) | 0;
	// 0x00 = automatic selection
	// 0x0A = 10  = 1.0Mbit/s
	// 0x14 = 20  = 2.0Mbit/s
	// 0x37 = 55  = 5.5Mbit/s
	// 0x6E = 110 = 11 Mbit/s

	// set TxControl
	header[6] = 0x0004;

	// write length
	// (not really needed since card will pull info from 802.3 header)
	//TxHeader[22] = len+8;

	// fill in 802.3 header fields
	TxHeader[30] = 0xAAAA;
	TxHeader[31] = 0x0003;
	TxHeader[32] = 0x0000;

	// src mac address @ byte offset 52
}

void prism2EventCheck(void)
{
	unsigned int evstat_data;

	evstat_data = prism2Read16(PRISM2_REG_EVSTAT);
	
	if(evstat_data & PRISM2_EVENT_TX)
	{
		prism2Write16(PRISM2_REG_EVACK,PRISM2_EVENT_TX);
	}

	if(evstat_data & PRISM2_EVENT_TXEXEC)
	{
		prism2Write16(PRISM2_REG_EVACK,PRISM2_EVENT_TXEXEC);
	}

	if(evstat_data & PRISM2_EVENT_ALLOC)
	{
		prism2Write16(PRISM2_REG_EVACK, 0x0002);
	}

	if(evstat_data & PRISM2_EVENT_CMD)
	{
		prism2Write16(PRISM2_REG_EVACK, PRISM2_EVENT_CMD);
	}

	if(evstat_data & PRISM2_EVENT_INFO)
	{
		prism2Read16(PRISM2_REG_INFOFID);
		prism2Write16(PRISM2_REG_EVACK, PRISM2_EVENT_INFO);
	}

	if(evstat_data & PRISM2_EVENT_INFDROP)
	{
		prism2Write16(PRISM2_REG_EVACK, PRISM2_EVENT_INFDROP);
	}

	if(evstat_data & PRISM2_EVENT_WTERR)
	{
		prism2Write16(PRISM2_REG_EVACK, PRISM2_EVENT_WTERR);
	}
}


unsigned int nicPoll(unsigned int maxlen, unsigned char* packet)
{
	u16 rxfid=0;
	u16 packetLength=0;

	// check if packets have been received
	if(prism2Read16(PRISM2_REG_EVSTAT) & PRISM2_EVENT_RX)
	{
		// we have a receive event
		// get RxFID
		rxfid = prism2Read16(PRISM2_REG_RXFID);
		// read the packet length
		prism2ReadBAP0(rxfid, 44, &packetLength, 1);
	}

	// if there's no packet or an error - exit
	if( !packetLength )
		return 0;
	
	// drop anything too big for the buffer
	if( packetLength > maxlen )
	{
		// ACK the receive event to finish up
		prism2Write16(PRISM2_REG_EVACK, PRISM2_EVENT_RX);
		return 0;
	}

	// packet is available, retrieve data
	// this is a hack: while reading in data,
	// convert 802.2/3 header to ethernet header
	// first get dest and src MAC addresses
	prism2ReadBAP0(rxfid, 46, (u16*)&packet[0], 6);
	// skip length, snap, and ctrl fields
	// begin data copy again at type field
	prism2ReadBAP0(rxfid, 46+12+8, (u16*)&packet[12], packetLength-6);
	// ACK the receive event to finish up
	prism2Write16(PRISM2_REG_EVACK, PRISM2_EVENT_RX);

	return packetLength;
}

void prism2InitPorts(void)
{
#if NIC_CONNECTION == MEMORY_MAPPED
  	// enable external SRAM interface - no wait states
  	sbi(MCUSR, SRE);
#else
	// set address port to output
	outb(PRISM2_ADDRESS_DDR, PRISM2_ADDRESS_MASK);
	outb(PRISM2_HADDRESS_DDR, PRISM2_HADDRESS_MASK);
    
	// set data port to input with pull-ups
	outb(PRISM2_DATA_DDR, 0x00);
	outb(PRISM2_DATA_PORT, 0xFF);

	// initialize the control port read and write pins to de-asserted
	sbi( PRISM2_CONTROL_PORT, PRISM2_CONTROL_IORD );
	sbi( PRISM2_CONTROL_PORT, PRISM2_CONTROL_IOWR );
	sbi( PRISM2_CONTROL_PORT, PRISM2_CONTROL_MEMRD );
	sbi( PRISM2_CONTROL_PORT, PRISM2_CONTROL_MEMWR );
	// set the read and write pins to output
	sbi( PRISM2_CONTROL_DDR, PRISM2_CONTROL_IORD );
	sbi( PRISM2_CONTROL_DDR, PRISM2_CONTROL_IOWR );
	sbi( PRISM2_CONTROL_DDR, PRISM2_CONTROL_MEMRD );
	sbi( PRISM2_CONTROL_DDR, PRISM2_CONTROL_MEMWR );
#endif
	// set reset pin to output
	sbi( PRISM2_RESET_DDR, PRISM2_RESET_PIN );

	// clear -REG pin
	sbi(DDRB, 6);
	cbi(PORTB, 6);
	// setup IREQ pin
	cbi(DDRB, 7);
	sbi(PORTB, 7);
}

void prism2Init(void)
{
	u08 result;
	u16 buffer[20];

//	rprintf("Init ports\r\n");
	prism2InitPorts();

	// assert hardware reset
	sbi( PRISM2_RESET_PORT, PRISM2_RESET_PIN );
	// wait
	delay_ms(10);
	// release hardware reset
	cbi( PRISM2_RESET_PORT, PRISM2_RESET_PIN );
	delay_ms(100);

/*
	// soft-reset card
	prism2WriteMem(0x3E0+PCMCIA_ATTR_COR, 0x80);
	delay_ms(10);
	prism2WriteMem(0x3E0+PCMCIA_ATTR_COR, 0x00);
	// wait until soft-reset is done
	delay_ms(500);

	// set 8-bit PCMCIA I/O mode
	prism2WriteMem(0x3E0+PCMCIA_ATTR_CSR, 0x20);

	prism2WriteMem(0x3E0+PCMCIA_ATTR_CSR, 0x04);
	timerPause(1000);
	prism2WriteMem(0x3E0+PCMCIA_ATTR_CSR, 0x00);
*/
	// enable PCMCIA I/O mode
	prism2WriteMem(0x3E0+PCMCIA_ATTR_COR, prism2ReadMem(0x3E0+PCMCIA_ATTR_COR) | 0x01);
//	prism2CardRegDump();

	rprintf("Prism2 Initializing...\r\n");
	if( (result = prism2Command(PRISM2_CMD_INIT,0)) )
	{
		rprintf("Prism2 Initialization Failure\r\n");
		rprintf("Result Code = %x\r\n",result);
	}
	
	rprintf("Prism2 Initialized\r\n");

	// set SSID
	prism2SetSSID("airdrop");

	// set max packet size
	buffer[0] = 0x0002;
	buffer[1] = PRISM2_RID_CNFMAXDATALEN;
	buffer[2] = 0x05DC;
	prism2WriteRID(PRISM2_RID_CNFMAXDATALEN, 0, buffer, 3);

	// set operating mode / port type
	buffer[0] = 0x0002;
	buffer[1] = PRISM2_RID_CNFPORTTYPE;
	//buffer[2] = 0x0000; // IBSS
	buffer[2] = 0x0001; // BSS
	prism2WriteRID(PRISM2_RID_CNFPORTTYPE, 0, buffer, 3);

	// set channel
//	buffer[0] = 0x0002;
//	buffer[1] = 0xFC03;
//	buffer[2] = 0x0001;
//	prism2WriteRID(0xFC00, 0, buffer, 3);

	// enable the interface
	prism2Command(PRISM2_CMD_ENABLE_MAC0,0);
}

void prism2Off(void)
{
	// turn off communication
	prism2Command(PRISM2_CMD_DISABLE_MAC0,0);
	// wait for all events to complete
	delay_ms(100);
	// reset card
	prism2Command(PRISM2_CMD_INIT,0);
}

void prism2GetMacAddress(u08* macaddr)
{
	u16 buffer[5];

	// read MAC address register
	prism2ReadRID(PRISM2_RID_CNFOWNMACADDR, 0, buffer, 5);

	*macaddr++ = buffer[2];
	*macaddr++ = buffer[2]>>8;
	*macaddr++ = buffer[3];
	*macaddr++ = buffer[3]>>8;
	*macaddr++ = buffer[4];
	*macaddr++ = buffer[4]>>8;
}

void prism2SetSSID(u08* ssid)
{
	u16 buffer[12];

	// prepare buffer for SSID write
	buffer[0] = 0x0012;
	buffer[1] = PRISM2_RID_CNFDESIREDSSID;
	buffer[2] = strlen(ssid);
	// copy ssid string to buffer
	strcpy((unsigned char*)&buffer[3], ssid);
	// write SSID
	prism2WriteRID(PRISM2_RID_CNFDESIREDSSID, 0, buffer, buffer[0]);
}

void prism2SetWEPKey(u08* wepkey)
{
	u16 buffer[9];

	// prepare buffer for SSID write
	buffer[0] = 0x0008;
	buffer[1] = PRISM2_RID_CNFWEPDEFAULTKEY0;
	// copy ssid string to buffer
	strncpy((unsigned char*)&buffer[2], wepkey, 13);
	buffer[8] &= 0x00FF;
	// write SSID
	prism2WriteRID(PRISM2_RID_CNFWEPDEFAULTKEY0, 0, buffer, buffer[0]);

	// set WEP active
	buffer[0] = 0x0002;
	buffer[1] = PRISM2_RID_CNFWEPFLAGS;
	buffer[2] = 0x0001;
	prism2WriteRID(PRISM2_RID_CNFWEPFLAGS, 0, buffer, buffer[0]);
	// set WEP active
	buffer[0] = 0x0002;
	buffer[1] = 0xfc2a;
	buffer[2] = 0x0001;
	prism2WriteRID(0xfc2a, 0, buffer, buffer[0]);
	// set WEP active
	buffer[0] = 0x0002;
	buffer[1] = 0xfc23;
	buffer[2] = 0x0000;
	prism2WriteRID(0xfc23, 0, buffer, buffer[0]);

}

u08 prism2Command(u16 cmd, u16 param0)
{
	u16 result;

	// wait until card not busy
//	rprintf("PRISM_CMD: Wait until card ready\r\n");
	while(prism2Read16(PRISM2_REG_CMD) & PRISM2_CMD_BUSY);
	
//	rprintf("PRISM_CMD: Issue Command = 0x%x\r\n", cmd);
	prism2Write16(PRISM2_REG_PARAM0, param0);
	prism2Write16(PRISM2_REG_CMD, cmd);
	
	// wait until card not busy
//	rprintf("PRISM_CMD: Wait until card ready\r\n");
	while(prism2Read16(PRISM2_REG_CMD) & PRISM2_CMD_BUSY);
	
	// read event register - wait for command to complete
//	rprintf("PRISM_CMD: Wait for command to complete\r\n");
	while(!(prism2Read16(PRISM2_REG_EVSTAT) & PRISM2_EVENT_CMD));

	// read status register
	result = prism2Read16(PRISM2_REG_STATUS)>>8;
//	rprintf("PRISM_CMD: Result  = 0x%x\r\n", result>>8);
//	rprintf("PRISM_CMD: CmdCode = 0x%x\r\n", result);
	
	// acknowledge event
//	rprintf("PRISM_CMD: Ack command event\r\n");
	prism2Write16(PRISM2_REG_EVACK, PRISM2_EVENT_CMD);

	// return command result
	return result;
}

u08 prism2ReadRID(u16 id, u16 offset, u16* data, u16 len)
{
	prism2Command(PRISM2_CMD_ACCESS_RD, id);
	return prism2ReadBAP0(id, offset, data, len);
}

u08 prism2WriteRID(u16 id, u16 offset, u16* data, u16 len)
{
	u08 result;
	result = prism2WriteBAP0(id, offset, data, len);
	prism2Command(PRISM2_CMD_ACCESS_WR, id);
	return result;
}

u08 prism2ReadBAP0(u16 id, u16 offset, u16* data, u16 len)
{
	// wait for BAP to be ready
	//rprintf("PRISM_BAP: Wait1\r\n");
	while( prism2Read16(PRISM2_REG_BAP0OFFSET) & PRISM2_BAPOFFSET_BUSY);
	// set ID
	prism2Write16(PRISM2_REG_BAP0SEL, id);
	// set offset
	prism2Write16(PRISM2_REG_BAP0OFFSET, offset);
	// wait for BAP to be ready
	//rprintf("PRISM_BAP: Wait2\r\n");
	while( prism2Read16(PRISM2_REG_BAP0OFFSET) & PRISM2_BAPOFFSET_BUSY);
	// check for error
	if(prism2Read16(PRISM2_REG_BAP0OFFSET) & PRISM2_BAPOFFSET_ERROR)
		return -1;
	// read data
	//rprintf("PRISM_BAP: Read\r\n");
	while(len--)
        *data++ = prism2Read16(PRISM2_REG_BAP0DATA);
	// return success
	return 0;
}

u08 prism2WriteBAP0(u16 id, u16 offset, u16* data, u16 len)
{
	// wait for BAP to be ready
	//rprintf("PRISM_BAP: Wait1\r\n");
	while( prism2Read16(PRISM2_REG_BAP0OFFSET) & PRISM2_BAPOFFSET_BUSY);
	// set ID
	prism2Write16(PRISM2_REG_BAP0SEL, id);
	// set offset
	prism2Write16(PRISM2_REG_BAP0OFFSET, offset);
	// wait for BAP to be ready
	//rprintf("PRISM_BAP: Wait2\r\n");
	while( prism2Read16(PRISM2_REG_BAP0OFFSET) & PRISM2_BAPOFFSET_BUSY);
	// check for error
	if(prism2Read16(PRISM2_REG_BAP0OFFSET) & PRISM2_BAPOFFSET_ERROR)
		return -1;
	// write data
	//rprintf("PRISM_BAP: Write\r\n");
	while(len--)
        prism2Write16(PRISM2_REG_BAP0DATA, *data++);
	// return success
	return 0;
}

void prism2Write(unsigned short address, unsigned char data)
{
	cli();
	// assert the address
	outb(PRISM2_HADDRESS_PORT, (address>>8) | (inb(PRISM2_HADDRESS_PORT)&~PRISM2_HADDRESS_MASK));
	outb(PRISM2_ADDRESS_PORT, address | (inb(PRISM2_ADDRESS_PORT)&~PRISM2_ADDRESS_MASK));
	// set data bus as output
	outb(PRISM2_DATA_DDR, 0xFF);
	// place data on bus
	outb(PRISM2_DATA_PORT, data);
	// assert write
	cbi(PRISM2_CONTROL_PORT, PRISM2_CONTROL_IOWR);
	// delay
	PRISM2_IO_ACCESS_DELAY;
	// negate write
	sbi(PRISM2_CONTROL_PORT, PRISM2_CONTROL_IOWR);
	// set data bus back to input with pullups enabled
	outb(PRISM2_DATA_DDR, 0x00);
	outb(PRISM2_DATA_PORT, 0xFF);
	sei();
}

unsigned char prism2Read(unsigned short address)
{
	unsigned char data;
	cli();
	// assert the address
	outb(PRISM2_HADDRESS_PORT, (address>>8) | (inb(PRISM2_HADDRESS_PORT)&~PRISM2_HADDRESS_MASK));
	outb(PRISM2_ADDRESS_PORT, address | (inb(PRISM2_ADDRESS_PORT)&~PRISM2_ADDRESS_MASK));
	// set data bus to input with pullups enabled
	outb(PRISM2_DATA_DDR, 0x00);
	outb(PRISM2_DATA_PORT, 0xFF);
	// assert read
	cbi(PRISM2_CONTROL_PORT, PRISM2_CONTROL_IORD);
	// delay
	PRISM2_IO_ACCESS_DELAY;
	// read in the data
	data = inb( PRISM2_DATA_PIN );
	// negate read
	sbi(PRISM2_CONTROL_PORT, PRISM2_CONTROL_IORD);
	// return data
	sei();
	return data;
}

void prism2Write16(unsigned short address, unsigned short data)
{
	prism2Write(address, data);
	prism2Write(address+1, data>>8);
}

unsigned short prism2Read16(unsigned short address)
{
	return prism2Read(address) | (prism2Read(address+1)<<8);
}

void prism2WriteMem(unsigned short address, unsigned short data)
{
	cli();
	// assert the address
	outb(PRISM2_HADDRESS_PORT, (address>>8) | (inb(PRISM2_HADDRESS_PORT)&~PRISM2_HADDRESS_MASK));
	outb(PRISM2_ADDRESS_PORT, address | (inb(PRISM2_ADDRESS_PORT)&~PRISM2_ADDRESS_MASK));
	// set data bus as output
	outb(PRISM2_DATA_DDR, 0xFF);
	// place data on bus
	outb(PRISM2_DATA_PORT, data);
	// assert write
	cbi(PRISM2_CONTROL_PORT, PRISM2_CONTROL_MEMWR);
	// delay
	PRISM2_MEM_ACCESS_DELAY;
	// negate write
	sbi(PRISM2_CONTROL_PORT, PRISM2_CONTROL_MEMWR);
	// set data bus back to input with pullups enabled
	outb(PRISM2_DATA_DDR, 0x00);
	outb(PRISM2_DATA_PORT, 0xFF);
	sei();
}

unsigned short prism2ReadMem(unsigned short address)
{
	unsigned char data;
	cli();
	// assert the address
	outb(PRISM2_HADDRESS_PORT, (address>>8) | (inb(PRISM2_HADDRESS_PORT)&~PRISM2_HADDRESS_MASK));
	outb(PRISM2_ADDRESS_PORT, address | (inb(PRISM2_ADDRESS_PORT)&~PRISM2_ADDRESS_MASK));
	// set data bus to input with pullups enabled
	outb(PRISM2_DATA_DDR, 0x00);
	outb(PRISM2_DATA_PORT, 0xFF);
	// assert read
	cbi(PRISM2_CONTROL_PORT, PRISM2_CONTROL_MEMRD);
	// delay
	PRISM2_MEM_ACCESS_DELAY;
	// read in the data
	data = inb( PRISM2_DATA_PIN );
	// negate read
	sbi(PRISM2_CONTROL_PORT, PRISM2_CONTROL_MEMRD);
	sei();
	// return data
	return data;
}

void prism2CardRegDump(void)
{
	u16 i;
	u08 buffer[0x100];

	rprintfProgStrM("Card Config Registers\r\n");
	rprintfProgStrM("-------------------------------\r\n");
	// read card CIS (16 bytes)
	rprintf("CIS : \r\n");
	for(i=0; i<0x100; i++)
		buffer[i] = prism2ReadMem(i<<1);
	debugPrintHexTable(0x100, buffer);

	rprintfCRLF();

	rprintf("COR : "); rprintfu08(prism2ReadMem(0x3E0+PCMCIA_ATTR_COR)); rprintfCRLF();
	rprintf("CSR : "); rprintfu08(prism2ReadMem(0x3E0+PCMCIA_ATTR_CSR)); rprintfCRLF();
	rprintf("PRR : "); rprintfu08(prism2ReadMem(0x3E0+PCMCIA_ATTR_PRR)); rprintfCRLF();
	rprintf("SCR : "); rprintfu08(prism2ReadMem(0x3E0+PCMCIA_ATTR_SCR)); rprintfCRLF();
}

void prism2RegDump(void)
{
	rprintfProgStrM("Prism2 Registers\r\n");
	rprintfProgStrM("CMD     : "); rprintfu16(prism2Read16(PRISM2_REG_CMD));		rprintfCRLF();
	rprintfProgStrM("PARAM0  : "); rprintfu16(prism2Read16(PRISM2_REG_PARAM0));		rprintfCRLF();
	rprintfProgStrM("PARAM1  : "); rprintfu16(prism2Read16(PRISM2_REG_PARAM1));		rprintfCRLF();
	rprintfProgStrM("PARAM2  : "); rprintfu16(prism2Read16(PRISM2_REG_PARAM2));		rprintfCRLF();
	rprintfProgStrM("STATUS  : "); rprintfu16(prism2Read16(PRISM2_REG_STATUS));		rprintfCRLF();
	rprintfProgStrM("RESP0   : "); rprintfu16(prism2Read16(PRISM2_REG_RESP0));		rprintfCRLF();
	rprintfProgStrM("RESP1   : "); rprintfu16(prism2Read16(PRISM2_REG_RESP1));		rprintfCRLF();
	rprintfProgStrM("RESP2   : "); rprintfu16(prism2Read16(PRISM2_REG_RESP2));		rprintfCRLF();

	rprintfProgStrM("INFOFID : "); rprintfu16(prism2Read16(PRISM2_REG_INFOFID));	rprintfCRLF();
	rprintfProgStrM("RXFID   : "); rprintfu16(prism2Read16(PRISM2_REG_RXFID));		rprintfCRLF();
	rprintfProgStrM("ALLOCFID: "); rprintfu16(prism2Read16(PRISM2_REG_ALLOCFID));	rprintfCRLF();
	rprintfProgStrM("TXFID   : "); rprintfu16(prism2Read16(PRISM2_REG_TXFID));		rprintfCRLF();

	rprintfProgStrM("BAP0SEL : "); rprintfu16(prism2Read16(PRISM2_REG_BAP0SEL));	rprintfCRLF();
	rprintfProgStrM("BAP0OFFS: "); rprintfu16(prism2Read16(PRISM2_REG_BAP0OFFSET));	rprintfCRLF();
	rprintfProgStrM("BAP0DATA: "); rprintfu16(prism2Read16(PRISM2_REG_BAP0DATA));	rprintfCRLF();

	rprintfProgStrM("BAP1SEL : "); rprintfu16(prism2Read16(PRISM2_REG_BAP1SEL));	rprintfCRLF();
	rprintfProgStrM("BAP1OFFS: "); rprintfu16(prism2Read16(PRISM2_REG_BAP1OFFSET));	rprintfCRLF();
	rprintfProgStrM("BAP1DATA: "); rprintfu16(prism2Read16(PRISM2_REG_BAP1DATA));	rprintfCRLF();

	rprintfProgStrM("EVSTAT  : "); rprintfu16(prism2Read16(PRISM2_REG_EVSTAT));		rprintfCRLF();
	rprintfProgStrM("INTEN   : "); rprintfu16(prism2Read16(PRISM2_REG_INTEN));		rprintfCRLF();
	rprintfProgStrM("EVACK   : "); rprintfu16(prism2Read16(PRISM2_REG_EVACK));		rprintfCRLF();

	rprintfProgStrM("SWSUP0  : "); rprintfu16(prism2Read16(PRISM2_REG_SWSUP0));		rprintfCRLF();
	rprintfProgStrM("SWSUP0  : "); rprintfu16(prism2Read16(PRISM2_REG_SWSUP1));		rprintfCRLF();
	rprintfProgStrM("SWSUP0  : "); rprintfu16(prism2Read16(PRISM2_REG_SWSUP2));		rprintfCRLF();

	rprintfProgStrM("AUXPAGE : "); rprintfu16(prism2Read16(PRISM2_REG_AUXPAGE));	rprintfCRLF();
	rprintfProgStrM("AUXOFFS : "); rprintfu16(prism2Read16(PRISM2_REG_AUXOFFSET));	rprintfCRLF();
	rprintfProgStrM("AUXDATA : "); rprintfu16(prism2Read16(PRISM2_REG_AUXDATA));	rprintfCRLF();

	delay_ms(25);
}
