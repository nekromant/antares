/*! \file rtl8019.c \brief Realtek RTL8019AS Ethernet Interface Driver. */
//*****************************************************************************
//
// File Name	: 'rtl8019.c'
// Title		: Realtek RTL8019AS Ethernet Interface Driver
// Author		: Pascal Stang
// Created		: 7/6/2004
// Revised		: 10/1/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// Based in part on code by Louis Beaudoin (www.embedded-creations.com).
// Thanks to Adam Dunkels and Louis Beaudoin for providing the initial
// structure in which to write this driver.
//*****************************************************************************

#include "global.h"
#include "timer.h"
#include "rprintf.h"

#include "rtl8019.h"

// include configuration
#include "rtl8019conf.h"

// pointers to locations in the RTL8019 receive buffer
static unsigned char NextPage;				// page pointer to next Rx packet
static unsigned int CurrentRetreiveAddress;	// DMA address for read Rx packet location


void nicInit(void)
{
	rtl8019Init();
}

void nicSend(unsigned int len, unsigned char* packet)
{
	rtl8019BeginPacketSend(len);
	rtl8019SendPacketData(packet, len);
	rtl8019EndPacketSend();
}

unsigned int nicPoll(unsigned int maxlen, unsigned char* packet)
{
	unsigned int packetLength;
	
	packetLength = rtl8019BeginPacketRetreive();

	// if there's no packet or an error - exit without ending the operation
	if( !packetLength )
		return 0;

	// drop anything too big for the buffer
	if( packetLength > maxlen )
	{
		rtl8019EndPacketRetreive();
		return 0;
	}
	
	// copy the packet data into the packet buffer
	rtl8019RetreivePacketData( packet, packetLength );
	rtl8019EndPacketRetreive();
		
	return packetLength;
}

void nicGetMacAddress(u08* macaddr)
{
	u08 tempCR;
	// switch register pages
	tempCR = rtl8019Read(CR);
	rtl8019Write(CR,tempCR|PS0);
	// read MAC address registers
	*macaddr++ = rtl8019Read(PAR0);
	*macaddr++ = rtl8019Read(PAR1);
	*macaddr++ = rtl8019Read(PAR2);
	*macaddr++ = rtl8019Read(PAR3);
	*macaddr++ = rtl8019Read(PAR4);
	*macaddr++ = rtl8019Read(PAR5);
	// switch register pages back
	rtl8019Write(CR,tempCR);
}

void nicSetMacAddress(u08* macaddr)
{
	u08 tempCR;
	// switch register pages
	tempCR = rtl8019Read(CR);
	rtl8019Write(CR,tempCR|PS0);
	// write MAC address registers
	rtl8019Write(PAR0, *macaddr++);
	rtl8019Write(PAR1, *macaddr++);
	rtl8019Write(PAR2, *macaddr++);
	rtl8019Write(PAR3, *macaddr++);
	rtl8019Write(PAR4, *macaddr++);
	rtl8019Write(PAR5, *macaddr++);
	// switch register pages back
	rtl8019Write(CR,tempCR);
}

void nicRegDump(void)
{
	rtl8019RegDump();
}


void rtl8019SetupPorts(void)
{
#if NIC_CONNECTION == MEMORY_MAPPED
	// enable external SRAM interface - no wait states
	sbi(MCUCR, SRE);
//	sbi(MCUCR, SRW10);
//	sbi(XMCRA, SRW00);
//	sbi(XMCRA, SRW01);
//	sbi(XMCRA, SRW11);
#else
    // make the address port output
	RTL8019_ADDRESS_DDR |= RTL8019_ADDRESS_MASK;
    // make the data port input with pull-ups
    RTL8019_DATA_PORT = 0xFF;

	// initialize the control port read and write pins to de-asserted
	RTL8019_CONTROL_DDR |= (1<<RTL8019_CONTROL_READPIN);
    RTL8019_CONTROL_DDR |= (1<<RTL8019_CONTROL_WRITEPIN);
	// set the read and write pins to output
	RTL8019_CONTROL_PORT |= (1<<RTL8019_CONTROL_READPIN);
	RTL8019_CONTROL_PORT |= (1<<RTL8019_CONTROL_WRITEPIN);
#endif
	// set reset pin to output
	sbi(RTL8019_RESET_DDR, RTL8019_RESET_PIN);
}


#if NIC_CONNECTION == MEMORY_MAPPED
inline void rtl8019Write(u08 address, u08 data)
{
	*(volatile u08*)(RTL8019_MEMORY_MAPPED_OFFSET + address) = data;
}
#else
void rtl8019Write(unsigned char address, unsigned char data)
{
	// assert the address
	RTL8019_ADDRESS_PORT = address | (RTL8019_ADDRESS_PORT&~RTL8019_ADDRESS_MASK);
	// set data bus as output and place data on bus
	RTL8019_DATA_DDR = 0xFF;
	RTL8019_DATA_PORT = data;
	// clock write pin
	cbi(RTL8019_CONTROL_PORT, RTL8019_CONTROL_WRITEPIN);
	nop();
	nop();
	sbi(RTL8019_CONTROL_PORT, RTL8019_CONTROL_WRITEPIN);
	// set data port back to input with pullups enabled
    RTL8019_DATA_DDR = 0x00;
    RTL8019_DATA_PORT = 0xFF;
}
#endif


#if NIC_CONNECTION == MEMORY_MAPPED
inline u08 ax88796Read(u08 address)
{
	return *(volatile u08*)(RTL8019_MEMORY_MAPPED_OFFSET + address);
}
#else
unsigned char rtl8019Read(unsigned char address)
{
	unsigned char data;
   
	// assert the address
	RTL8019_ADDRESS_PORT = address | (RTL8019_ADDRESS_PORT&~RTL8019_ADDRESS_MASK);
	// assert read
	cbi(RTL8019_CONTROL_PORT, RTL8019_CONTROL_READPIN);
	nop();
	nop();
	// read in the data
	data = RTL8019_DATA_PIN;
	// negate read
	sbi(RTL8019_CONTROL_PORT, RTL8019_CONTROL_READPIN);
	// return data
	return data;
}
#endif                       


void rtl8019Init(void)
{
	// setup I/O ports
	rtl8019SetupPorts();
	
	// do a hard reset
	sbi(RTL8019_RESET_PORT, RTL8019_RESET_PIN);
	delay_ms(10);
	cbi(RTL8019_RESET_PORT, RTL8019_RESET_PIN);

	// clear interrupt state
	rtl8019Write( ISR, rtl8019Read(ISR) );
	delay_ms(50);

	// switch to page 3 to load config registers
	rtl8019Write(CR, (PS0|PS1|RD2|STOP));

	// disable EEPROM write protect of config registers
	rtl8019Write(RTL_EECR, (EEM1|EEM0));

    // set network type to 10 Base-T link test
	rtl8019Write(CONFIG2, 0x20);

    // disable powerdown and sleep
	rtl8019Write(CONFIG3, 0);
	delay_ms(255);

    // reenable EEPROM write protect
	rtl8019Write(RTL_EECR, 0);

    // go back to page 0, stop NIC, abort DMA
	rtl8019Write(CR, (RD2|STOP));
	delay_ms(2);					// wait for traffic to complete
	rtl8019Write(DCR, DCR_INIT);
	rtl8019Write(RBCR0,0x00);
	rtl8019Write(RBCR1,0x00);
	rtl8019Write(RCR, AB);
	rtl8019Write(TPSR, TXSTART_INIT);
	rtl8019Write(TCR, LB0);
	rtl8019Write(PSTART, RXSTART_INIT);
	rtl8019Write(BNRY, RXSTART_INIT);
	rtl8019Write(PSTOP, RXSTOP_INIT);
	rtl8019Write(CR, (PS0|RD2|STOP));	// switch to page 1
	delay_ms(2);
	rtl8019Write(CPR, RXSTART_INIT);
	
	// set MAC address
	rtl8019Write(PAR0, RTL8019_MAC0);
	rtl8019Write(PAR1, RTL8019_MAC1);
	rtl8019Write(PAR2, RTL8019_MAC2);
	rtl8019Write(PAR3, RTL8019_MAC3);
	rtl8019Write(PAR4, RTL8019_MAC4);
	rtl8019Write(PAR5, RTL8019_MAC5);
    
	// initialize sequence per NE2000 spec
	rtl8019Write(CR, (RD2|STOP));
	rtl8019Write(DCR, DCR_INIT);
	rtl8019Write(CR, (RD2|START));
	rtl8019Write(ISR,0xFF);			// clear all interrupts
	rtl8019Write(IMR, IMR_INIT);
	rtl8019Write(TCR, TCR_INIT);
	
	rtl8019Write(CR, (RD2|START));	// start the NIC
}


void rtl8019BeginPacketSend(unsigned int packetLength)
{
	unsigned int sendPacketLength;
	sendPacketLength = (packetLength>=ETHERNET_MIN_PACKET_LENGTH)?
						(packetLength):ETHERNET_MIN_PACKET_LENGTH;
	
	//start the NIC
	rtl8019Write(CR, (RD2|START));
	
	// still transmitting a packet - wait for it to finish
	while( rtl8019Read(CR) & TXP );

	// load beginning page for transmit buffer
	rtl8019Write(TPSR,TXSTART_INIT);
	
	// set start address for remote DMA operation
	rtl8019Write(RSAR0,0x00);
	rtl8019Write(RSAR1,0x40);
	
	// clear the packet stored interrupt
	rtl8019Write(ISR,PTX);

	// load data byte count for remote DMA
	rtl8019Write(RBCR0, (unsigned char)(packetLength));
	rtl8019Write(RBCR1, (unsigned char)(packetLength>>8));

	rtl8019Write(TBCR0, (unsigned char)(sendPacketLength));
	rtl8019Write(TBCR1, (unsigned char)((sendPacketLength)>>8));
	
	// do remote write operation
	rtl8019Write(CR,(RD1|START));
}


void rtl8019SendPacketData(unsigned char *localBuffer, unsigned int length)
{
	unsigned int i;
	
	// write data to DMA port
	for(i=0;i<length;i++)
		rtl8019Write(RDMAPORT, localBuffer[i]);
}


void rtl8019EndPacketSend(void)
{
	//send the contents of the transmit buffer onto the network
	rtl8019Write(CR,(RD2|TXP));
	// clear the remote DMA interrupt
	rtl8019Write(ISR, RDC);
}


unsigned int rtl8019BeginPacketRetreive(void)
{
	unsigned char i;
	unsigned char bnry;
	
	unsigned char pageheader[4];
	unsigned int rxlen;
	
	// check for and handle an overflow
	rtl8019ProcessInterrupt();
	
	// read CPR from page 1
	rtl8019Write(CR,(PS0|RD2|START));
	i = rtl8019Read(CPR);
	
	// return to page 0
	rtl8019Write(CR,(RD2|START));
	
	// read the boundary register - pointing to the beginning of the packet
	bnry = rtl8019Read(BNRY) ;
	
	// return if there is no packet in the buffer
	if( bnry == i )
		return 0;

	// clear the packet received interrupt flag
	rtl8019Write(ISR, PRX);
	
	// if boundary pointer is invalid
	if( (bnry >= RXSTOP_INIT) || (bnry < RXSTART_INIT) )
	{
		// reset the contents of the buffer and exit
		rtl8019Write(BNRY, RXSTART_INIT);
		rtl8019Write(CR, (PS0|RD2|START));
		rtl8019Write(CPR, RXSTART_INIT);
		rtl8019Write(CR, (RD2|START));
		return 0;
	}

	// initiate DMA to transfer the RTL8019 packet header
	rtl8019Write(RBCR0, 4);
	rtl8019Write(RBCR1, 0);
	rtl8019Write(RSAR0, 0);
	rtl8019Write(RSAR1, bnry);
	rtl8019Write(CR, (RD0|START));
	// transfer packet header
	for(i=0;i<4;i++)
		pageheader[i] = rtl8019Read(RDMAPORT);
	// end the DMA operation
	rtl8019Write(CR, (RD2|START));
	// wait for remote DMA complete
	for(i = 0; i < 20; i++)
		if(rtl8019Read(ISR) & RDC)
			break;
	rtl8019Write(ISR, RDC);

	rxlen = (pageheader[PKTHEADER_PKTLENH]<<8) + pageheader[PKTHEADER_PKTLENL];
	NextPage = pageheader[PKTHEADER_NEXTPAGE];
	
	CurrentRetreiveAddress = (bnry<<8) + 4;
	
	// if the NextPage pointer is invalid, the packet is not ready yet - exit
	if( (NextPage >= RXSTOP_INIT) || (NextPage < RXSTART_INIT) )
		return 0;
    
	return rxlen-4;
}


void rtl8019RetreivePacketData(unsigned char * localBuffer, unsigned int length)
{
	unsigned int i;
	
	// initiate DMA to transfer the data
	rtl8019Write(RBCR0, (unsigned char)length);
	rtl8019Write(RBCR1, (unsigned char)(length>>8));
	rtl8019Write(RSAR0, (unsigned char)CurrentRetreiveAddress);
	rtl8019Write(RSAR1, (unsigned char)(CurrentRetreiveAddress>>8));
	rtl8019Write(CR, (RD0|START));
	// transfer packet data
	for(i=0;i<length;i++)
		localBuffer[i] = rtl8019Read(RDMAPORT);
	// end the DMA operation
	rtl8019Write(CR, (RD2|START));
	// wait for remote DMA complete
	for(i=0; i<20; i++)
		if(rtl8019Read(ISR) & RDC)
			break;
	rtl8019Write(ISR, RDC);
	// keep track of current address
    CurrentRetreiveAddress += length;
    if( CurrentRetreiveAddress >= 0x6000 )
    	CurrentRetreiveAddress = CurrentRetreiveAddress - (0x6000-0x4600) ;
}


void rtl8019EndPacketRetreive(void)
{
	unsigned char i;

	// end the DMA operation
	rtl8019Write(CR, (RD2|START));
	// wait for remote DMA complete
	for(i=0; i<20; i++)
		if(rtl8019Read(ISR) & RDC)
			break;
	rtl8019Write(ISR, RDC);

	// set the boundary register to point to the start of the next packet
	rtl8019Write(BNRY, NextPage);
}


void rtl8019ProcessInterrupt(void)
{
	unsigned char byte = rtl8019Read(ISR);
	
	if( byte & OVW )
		rtl8019ReceiveOverflowRecover();
}

void rtl8019ReceiveOverflowRecover(void)
{
	unsigned char data_L, resend;	

	data_L = rtl8019Read(CR);
	rtl8019Write(CR, 0x21);
	delay_ms(2);
	rtl8019Write(RBCR0, 0x00);
	rtl8019Write(RBCR1, 0x00);
	if(!(data_L & 0x04))
		resend = 0;
	else if(data_L & 0x04)
	{
		data_L = rtl8019Read(ISR);
		if((data_L & 0x02) || (data_L & 0x08))
	    	resend = 0;
	    else
	    	resend = 1;
	}
	
	rtl8019Write(TCR, 0x02);
	rtl8019Write(CR, 0x22);
	rtl8019Write(BNRY, RXSTART_INIT);
	rtl8019Write(CR, 0x62);
	rtl8019Write(CPR, RXSTART_INIT);
	rtl8019Write(CR, 0x22);
	rtl8019Write(ISR, 0x10);
	rtl8019Write(TCR, TCR_INIT);
	
    if(resend)
        rtl8019Write(CR, 0x26);

    rtl8019Write(ISR, 0xFF);
}


void rtl8019RegDump(void)
{
//	unsigned char result;
//	result = rtl8019Read(TR);
	
//	rprintf("Media State: ");
//	if(!(result & AUTOD))
//   		rprintf("Autonegotiation\r\n");
//	else if(result & RST_B)
//   		rprintf("PHY in Reset   \r\n");
//	else if(!(result & RST_10B))
//		rprintf("10BASE-T       \r\n");
//	else if(!(result & RST_TXB))
//		rprintf("100BASE-T      \r\n");
				
	//rprintf("TR regsiter      : %x\r\n",result);
	//result = read_mii(0x10,0);
	//rprintf("MII regsiter 0x10: %x\r\n",result);

	rprintf("Page0: CR  BNRY PSR PST ISR TSR RSR MMR TR  GPI\r\n");
	rprintfProgStrM("       ");
	rprintfu08(rtl8019Read(CR));
	rprintfProgStrM("  ");
	rprintfu08(rtl8019Read(BNRY));
	rprintfProgStrM("   ");
	rprintfu08(rtl8019Read(PSTART));
	rprintfProgStrM("  ");
	rprintfu08(rtl8019Read(PSTOP));
	rprintfProgStrM("  ");
	rprintfu08(rtl8019Read(ISR));
	rprintfProgStrM("  ");
	rprintfu08(rtl8019Read(TSR));
	rprintfProgStrM("  ");
	rprintfu08(rtl8019Read(RSR));
	rprintfProgStrM("  ");
//	rprintfu08(rtl8019Read(MEMR));
	rprintfProgStrM("  ");
//	rprintfu08(rtl8019Read(TR));
	rprintfProgStrM("  ");
//	rprintfu08(rtl8019Read(GPI));
	rprintfCRLF();

	rtl8019Write(CR,rtl8019Read(CR)|PS0);

	rprintf("Page1: CR  PAR    CPR\r\n");
	rprintfProgStrM("       ");
	rprintfu08(rtl8019Read(CR));
	rprintfProgStrM("  ");
	rprintfChar(rtl8019Read(PAR0));
	rprintfChar(rtl8019Read(PAR1));
	rprintfChar(rtl8019Read(PAR2));
	rprintfChar(rtl8019Read(PAR3));
	rprintfChar(rtl8019Read(PAR4));
	rprintfChar(rtl8019Read(PAR5));
	rprintfProgStrM(" ");
	rprintfu08(rtl8019Read(CPR));
	
	rtl8019Write(CR,rtl8019Read(CR)&~PS0);

	delay_ms(25);
}

