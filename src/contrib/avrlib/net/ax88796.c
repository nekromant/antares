/*! \file ax88796.c \brief ASIX AX88796 Ethernet Interface Driver. */
//*****************************************************************************
//
// File Name	: 'ax88796.c'
// Title		: ASIX AX88796 Ethernet Interface Driver
// Author		: Pascal Stang
// Created		: 10/22/2002
// Revised		: 8/21/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// Description	: This driver provides initialization and transmit/receive
//		functions for the ASIX AX88796 10/100Mb Ethernet Controller and PHY.
//
// Based in part on code by Louis Beaudoin (www.embedded-creations.com).
// Thanks to Adam Dunkels and Louis Beaudoin for providing the initial
// structure in which to write this driver.
//
//*****************************************************************************

#include "global.h"
#include "timer.h"
#include "rprintf.h"

#include "ax88796.h"

// include configuration
#include "ax88796conf.h"

// pointers to locations in the ax88796 receive buffer
static unsigned char NextPage;				// page pointer to next Rx packet
static unsigned int CurrentRetreiveAddress;	// DMA address for read Rx packet location


void nicInit(void)
{
	ax88796Init();
}

void nicSend(unsigned int len, unsigned char* packet)
{
	ax88796BeginPacketSend(len);
	ax88796SendPacketData(packet, len);
	ax88796EndPacketSend();
}

unsigned int nicPoll(unsigned int maxlen, unsigned char* packet)
{
	unsigned int packetLength;
	
	packetLength = ax88796BeginPacketRetreive();

	// if there's no packet or an error - exit without ending the operation
	if( !packetLength )
		return 0;

	// drop anything too big for the buffer
	if( packetLength > maxlen )
	{
		ax88796EndPacketRetreive();
		return 0;
	}
	
	// copy the packet data into the uIP packet buffer
	ax88796RetreivePacketData( packet, packetLength );
	ax88796EndPacketRetreive();
		
	return packetLength;
}

void nicGetMacAddress(u08* macaddr)
{
	u08 tempCR;
	// switch register pages
	tempCR = ax88796Read(CR);
	ax88796Write(CR,tempCR|PS0);
	// read MAC address registers
	*macaddr++ = ax88796Read(PAR0);
	*macaddr++ = ax88796Read(PAR1);
	*macaddr++ = ax88796Read(PAR2);
	*macaddr++ = ax88796Read(PAR3);
	*macaddr++ = ax88796Read(PAR4);
	*macaddr++ = ax88796Read(PAR5);
	// switch register pages back
	ax88796Write(CR,tempCR);
}

void nicSetMacAddress(u08* macaddr)
{
	u08 tempCR;
	// switch register pages
	tempCR = ax88796Read(CR);
	ax88796Write(CR,tempCR|PS0);
	// write MAC address registers
	ax88796Write(PAR0, *macaddr++);
	ax88796Write(PAR1, *macaddr++);
	ax88796Write(PAR2, *macaddr++);
	ax88796Write(PAR3, *macaddr++);
	ax88796Write(PAR4, *macaddr++);
	ax88796Write(PAR5, *macaddr++);
	// switch register pages back
	ax88796Write(CR,tempCR);
}

void nicRegDump(void)
{
	ax88796RegDump();
}


void ax88796SetupPorts(void)
{
#if NIC_CONNECTION == MEMORY_MAPPED
	// enable external SRAM interface - no wait states
	sbi(MCUCR, SRE);
//	sbi(MCUCR, SRW10);
//	sbi(XMCRA, SRW00);
//	sbi(XMCRA, SRW01);
//	sbi(XMCRA, SRW11);
#else
	// set address port to output
	AX88796_ADDRESS_DDR = AX88796_ADDRESS_MASK;
    
	// set data port to input with pull-ups
	AX88796_DATA_DDR = 0x00;
	AX88796_DATA_PORT = 0xFF;

	// initialize the control port read and write pins to de-asserted
	sbi( AX88796_CONTROL_PORT, AX88796_CONTROL_READPIN );
	sbi( AX88796_CONTROL_PORT, AX88796_CONTROL_WRITEPIN );
	// set the read and write pins to output
	sbi( AX88796_CONTROL_DDR, AX88796_CONTROL_READPIN );
	sbi( AX88796_CONTROL_DDR, AX88796_CONTROL_WRITEPIN );
#endif
	// set reset pin to output
	sbi( AX88796_RESET_DDR, AX88796_RESET_PIN );
}


#if NIC_CONNECTION == MEMORY_MAPPED
inline void ax88796Write(u08 address, u08 data)
{
	*(volatile u08*)(AX88796_MEMORY_MAPPED_OFFSET + address) = data;
}
#else
void ax88796Write(u08 address, u08 data)
{
	// assert the address
    AX88796_ADDRESS_PORT = address | (AX88796_ADDRESS_PORT&~AX88796_ADDRESS_MASK);

	// set data bus as output and place data on bus
	AX88796_DATA_DDR = 0xFF;
	AX88796_DATA_PORT = data;
    
	// clock write pin
	cbi(AX88796_CONTROL_PORT, AX88796_CONTROL_WRITEPIN);
	nop();
	nop();
	sbi(AX88796_CONTROL_PORT, AX88796_CONTROL_WRITEPIN);
    
	// set data bus back to input with pullups enabled
	AX88796_DATA_DDR = 0x00;
	AX88796_DATA_PORT = 0xFF;
}
#endif


#if NIC_CONNECTION == MEMORY_MAPPED
inline u08 ax88796Read(u08 address)
{
	return *(volatile u08*)(AX88796_MEMORY_MAPPED_OFFSET + address);
}
#else
u08 ax88796Read(u08 address)
{
	u08 data;
   
	// assert the address
    AX88796_ADDRESS_PORT = address | (AX88796_ADDRESS_PORT&~AX88796_ADDRESS_MASK);

	// assert read
	cbi(AX88796_CONTROL_PORT, AX88796_CONTROL_READPIN);
	nop();
	nop();
	// read in the data
	data = AX88796_DATA_PIN;

	// negate read
	sbi(AX88796_CONTROL_PORT, AX88796_CONTROL_READPIN);

	return data;
}
#endif                       


void ax88796Init(void)
{
	unsigned char tcrFduFlag;
	
	// initialize I/O ports
	ax88796SetupPorts();

	// do a hard reset
	sbi(AX88796_RESET_PORT, AX88796_RESET_PIN);
	delay_ms(100);
	cbi(AX88796_RESET_PORT, AX88796_RESET_PIN);

	// do soft reset
	ax88796Write(ISR, ax88796Read(ISR));
	delay_ms(50);

	// wait for PHY to come out of reset
	ax88796Read(RSTPORT);
	while(ax88796Read(TR) & RST_B);
  
	ax88796WriteMii(0x10,0x00,0x0800);
	delay_ms(255);
	ax88796WriteMii(0x10,0x00,0x1200);

	ax88796Write(CR,(RD2|STOP));		// stop the NIC, abort DMA, page 0
	delay_ms(5);						// make sure nothing is coming in or going out
	ax88796Write(DCR,DCR_INIT);    
	ax88796Write(RBCR0,0x00);
	ax88796Write(RBCR1,0x00);
	ax88796Write(IMR,0x00);
	ax88796Write(ISR,0xFF);
	ax88796Write(RCR,0x20);
	ax88796Write(BNRY,RXSTART_INIT);
	ax88796Write(PSTART,RXSTART_INIT);
	ax88796Write(PSTOP,RXSTOP_INIT);
	
	// switch to page 1
	ax88796Write(CR,(PS0|RD2|STOP));
	// write mac address
	ax88796Write(PAR0+0, AX88796_MAC0);
	ax88796Write(PAR0+1, AX88796_MAC1);
	ax88796Write(PAR0+2, AX88796_MAC2);
	ax88796Write(PAR0+3, AX88796_MAC3);
	ax88796Write(PAR0+4, AX88796_MAC4);
	ax88796Write(PAR0+5, AX88796_MAC5);
	// set start point
	ax88796Write(CURR,RXSTART_INIT+1);

	ax88796Write(CR,(RD2|START));
	ax88796Write(RCR,RCR_INIT);

	if(ax88796Read(GPI) & I_SPD)		// check PHY speed setting
		tcrFduFlag = FDU;				// if 100base, do full duplex
	else
		tcrFduFlag = 0;					// if 10base, do half duplex
		
	ax88796Write(TCR,(tcrFduFlag|TCR_INIT));

	ax88796Write(GPOC,MPSEL);			// select media interface
  
	ax88796Write(TPSR,TXSTART_INIT);

	ax88796Write(CR,(RD2|STOP));
	ax88796Write(DCR,DCR_INIT);
	ax88796Write(CR,(RD2|START));
	ax88796Write(ISR,0xFF);
	ax88796Write(IMR,IMR_INIT);
	ax88796Write(TCR,(tcrFduFlag|TCR_INIT));

	//test
/*
	while(1)
	{
		vt100SetCursorPos(18,0);
		ax88796RegDump();
	}
*/
}


void ax88796BeginPacketSend(unsigned int packetLength)
{
	unsigned int sendPacketLength;
	sendPacketLength = (packetLength>=ETHERNET_MIN_PACKET_LENGTH)?
						(packetLength):(ETHERNET_MIN_PACKET_LENGTH);
	
	//start the NIC
	ax88796Write(CR,(RD2|START));
	
	// still transmitting a packet - wait for it to finish
	while( ax88796Read(CR) & TXP );

	//load beginning page for transmit buffer
	ax88796Write(TPSR,TXSTART_INIT);
	
	//set start address for remote DMA operation
	ax88796Write(RSAR0,0x00);
	ax88796Write(RSAR1,0x40);
	
	//clear the packet stored interrupt
	ax88796Write(ISR, PTX);

	//load data byte count for remote DMA
	ax88796Write(RBCR0, (unsigned char)(packetLength));
	ax88796Write(RBCR1, (unsigned char)(packetLength>>8));

	ax88796Write(TBCR0, (unsigned char)(sendPacketLength));
	ax88796Write(TBCR1, (unsigned char)((sendPacketLength)>>8));
	
	//do remote write operation
	ax88796Write(CR,0x12);
}


void ax88796SendPacketData(unsigned char * localBuffer, unsigned int length)
{
	unsigned int i;
	
	for(i=0;i<length;i++)
		ax88796Write(RDMAPORT, localBuffer[i]);
}


void ax88796EndPacketSend(void)
{
	//send the contents of the transmit buffer onto the network
	ax88796Write(CR,(RD2|TXP));
	
	// clear the remote DMA interrupt
	ax88796Write(ISR, RDC);
}


unsigned int ax88796BeginPacketRetreive(void)
{
	unsigned char writePagePtr;
	unsigned char readPagePtr;
	unsigned char bnryPagePtr;
	unsigned char i;
	
	unsigned char pageheader[4];
	unsigned int rxlen;
	
	// check for and handle an overflow
	ax88796ProcessInterrupt();
	
	// read CURR from page 1
	ax88796Write(CR,(PS0|RD2|START));
	writePagePtr = ax88796Read(CURR);
	// read the boundary register from page 0
	ax88796Write(CR,(RD2|START));
	bnryPagePtr = ax88796Read(BNRY);

	// first packet is at page bnryPtr+1
	readPagePtr = bnryPagePtr+1;
	if(readPagePtr >= RXSTOP_INIT) readPagePtr = RXSTART_INIT;
	
	// return if there is no packet in the buffer
	if( readPagePtr == writePagePtr )
	{
		return 0;
	}
	
	// clear the packet received interrupt flag
	ax88796Write(ISR, PRX);
	
	// if the boundary pointer is invalid,
	// reset the contents of the buffer and exit
	if( (bnryPagePtr < RXSTART_INIT) || (bnryPagePtr >= RXSTOP_INIT) )
	{
		ax88796Write(BNRY, RXSTART_INIT);
		ax88796Write(CR, (PS0|RD2|START));
		ax88796Write(CURR, RXSTART_INIT+1);
		ax88796Write(CR, (RD2|START));
		
//		rprintf("B");
		return 0;
	}

	// initiate DMA to transfer the RTL8019 packet header
	ax88796Write(RBCR0, 4);
	ax88796Write(RBCR1, 0);
	ax88796Write(RSAR0, 0);
	ax88796Write(RSAR1, readPagePtr);
	ax88796Write(CR, (RD0|START));
	for(i=0;i<4;i++)
		pageheader[i] = ax88796Read(RDMAPORT);

	// end the DMA operation
    ax88796Write(CR, (RD2|START));
    for(i = 0; i <= 20; i++)
        if(ax88796Read(ISR) & RDC)
            break;
    ax88796Write(ISR, RDC);
	
	rxlen = (pageheader[PKTHEADER_PKTLENH]<<8) + pageheader[PKTHEADER_PKTLENL];
	NextPage = pageheader[PKTHEADER_NEXTPAGE];
	
	CurrentRetreiveAddress = (readPagePtr<<8) + 4;
	
	// if the NextPage pointer is invalid, the packet is not ready yet - exit
	if( (NextPage >= RXSTOP_INIT) || (NextPage < RXSTART_INIT) )
	{
//		rprintf("N");
//		rprintfu08(nextPage);
		return 0;
	}

    return rxlen-4;
}


void ax88796RetreivePacketData(unsigned char * localBuffer, unsigned int length)
{
	unsigned int i;
	
	// initiate DMA to transfer the data
	ax88796Write(RBCR0, (unsigned char)length);
	ax88796Write(RBCR1, (unsigned char)(length>>8));
	ax88796Write(RSAR0, (unsigned char)CurrentRetreiveAddress);
	ax88796Write(RSAR1, (unsigned char)(CurrentRetreiveAddress>>8));
	ax88796Write(CR, (RD0|START));
	for(i=0;i<length;i++)
		localBuffer[i] = ax88796Read(RDMAPORT);

	// end the DMA operation
	ax88796Write(CR, (RD2|START));
	for(i = 0; i <= 20; i++)
		if(ax88796Read(ISR) & RDC)
			break;
	ax88796Write(ISR, RDC);
    
	CurrentRetreiveAddress += length;
	if( CurrentRetreiveAddress >= 0x6000 )
    	CurrentRetreiveAddress -= (0x6000-0x4600) ;
}


void ax88796EndPacketRetreive(void)
{
	unsigned char i;
	unsigned char bnryPagePtr;

	// end the DMA operation
    ax88796Write(CR, (RD2|START));
    for(i = 0; i <= 20; i++)
        if(ax88796Read(ISR) & RDC)
            break;
    ax88796Write(ISR, RDC);

	// set the boundary register to point
	// to the start of the next packet-1
    bnryPagePtr = NextPage-1;
	if(bnryPagePtr < RXSTART_INIT) bnryPagePtr = RXSTOP_INIT-1;

	ax88796Write(BNRY, bnryPagePtr);
}


void ax88796ProcessInterrupt(void)
{
	unsigned char intr = ax88796Read(ISR);
	
	// check for receive overflow
	if( intr & OVW )
		ax88796ReceiveOverflowRecover();
}


void ax88796ReceiveOverflowRecover(void)
{
	// receive buffer overflow handling procedure
	// as specified in the AX88796 datasheet

	unsigned char cmdReg;
	unsigned char resend=0;

	// check if we were transmitting something
	cmdReg = ax88796Read(CR);
	// stop the interface
	ax88796Write(CR, (RD2|STOP));
	// wait for timeout
	delay_ms(2);
	// clear remote byte count registers
	ax88796Write(RBCR0, 0x00);
	ax88796Write(RBCR1, 0x00);
	
	// if we were transmitting something
	if(cmdReg & TXP)
	{
		// check if the transmit completed
		cmdReg = ax88796Read(ISR);
		if((cmdReg & PTX) || (cmdReg & TXE))
			resend = 0;		// transmit completed
	    else
			resend = 1;		// transmit was interrupted, must resend
	}
	// switch to loopback mode
	ax88796Write(TCR, LB0);
	// start the interface
	ax88796Write(CR, (RD2|START));
	// set boundary
	ax88796Write(BNRY, RXSTART_INIT);
	// go to page 1
	ax88796Write(CR, (PS0|RD2|START));
	// set current page register
	ax88796Write(CPR, RXSTART_INIT+1);
	// go to page 0
	ax88796Write(CR, (RD2|START));
	// clear the overflow int
	ax88796Write(ISR, OVW);
	// switch to normal (non-loopback mode)
	ax88796Write(TCR, TCR_INIT);

	// if previous transmit was interrupted, then resend
	if(resend)
		ax88796Write(CR, (RD2|TXP|START));

	// recovery completed
}


#define set_mdc		ax88796Write(MEMR,ax88796Read(MEMR)|0x01);
#define clr_mdc		ax88796Write(MEMR,ax88796Read(MEMR)&0xFE);

#define mii_clk		set_mdc; clr_mdc;				  
					
#define set_mdir	ax88796Write(MEMR,ax88796Read(MEMR)|0x02);
#define clr_mdir	ax88796Write(MEMR,ax88796Read(MEMR)&0xFD);
					
#define set_mdo		ax88796Write(MEMR,ax88796Read(MEMR)|0x08)
#define clr_mdo		ax88796Write(MEMR,ax88796Read(MEMR)&0xF7)

#define mii_write	clr_mdo; mii_clk;	\
					set_mdo; mii_clk;	\
					clr_mdo; mii_clk;	\
					set_mdo; mii_clk;

#define mii_read	clr_mdo; mii_clk;	\
					set_mdo; mii_clk;	\
					set_mdo; mii_clk;	\
					clr_mdo; mii_clk;

#define mii_r_ta    mii_clk;			\

#define mii_w_ta    set_mdo; mii_clk;	\
					clr_mdo; mii_clk;
			
void ax88796WriteMii(unsigned char phyad,unsigned char regad,unsigned int mii_data)
{
	unsigned char mask8;
	unsigned int  i,mask16;

	mii_write;
 
	mask8 = 0x10;
	for(i=0;i<5;++i)
	{
  	   	if(mask8 & phyad)
			set_mdo;
		else
			clr_mdo;
		mii_clk;
		mask8 >>= 1;	 
	}   
	mask8 = 0x10;
	for(i=0;i<5;++i)
	{
  		if(mask8 & regad)
			set_mdo;
		else
			clr_mdo;
		mii_clk;
		mask8 >>= 1;	 
	}    					
	mii_w_ta;
 
	mask16 = 0x8000;
	for(i=0;i<16;++i)
	{
		if(mask16 & mii_data)
			set_mdo;
		else
			clr_mdo;
		mii_clk;	 
		mask16 >>= 1;	 
	}   			
}
 
unsigned int ax88796ReadMii(unsigned char phyad,unsigned char regad)
{
	unsigned char mask8,i;
	unsigned int  mask16,result16;
 
	mii_read;

	mask8 = 0x10;
	for(i=0;i<5;++i)
	{
		if(mask8 & phyad)
			set_mdo;
		else
			clr_mdo;
		mii_clk;	 
		mask8 >>= 1;
	}
	mask8 = 0x10;
	for(i=0;i<5;++i)
	{
		if(mask8 & regad)
			set_mdo;
		else
			clr_mdo;
		mii_clk;
		mask8 >>= 1;
	}
   			
	mii_r_ta;
 
	mask16 = 0x8000;
	result16 = 0x0000;
	for(i=0;i<16;++i)
	{
		mii_clk;
		if(ax88796Read(MEMR) & 0x04)
		{
			result16 |= mask16;
		}
		else
		{
			asm volatile ("nop");
			break;
		}
		mask16 >>= 1;
	}
	return result16;
}


void ax88796RegDump(void)
{
	unsigned char result;
	result = ax88796Read(TR);
	
	rprintf("Media State: ");
	if(!(result & AUTOD))
   		rprintf("Autonegotiation\r\n");
	else if(result & RST_B)
   		rprintf("PHY in Reset   \r\n");
	else if(!(result & RST_10B))
		rprintf("10BASE-T       \r\n");
	else if(!(result & RST_TXB))
		rprintf("100BASE-T      \r\n");
				
	//rprintf("TR regsiter      : %x\r\n",result);
	//result = read_mii(0x10,0);
	//rprintf("MII regsiter 0x10: %x\r\n",result);

	rprintfProgStrM("Page0: CR  BNRY PSR PST ISR TSR RSR MMR TR  GPI\r\n");
	rprintfProgStrM("       ");
	rprintfu08(ax88796Read(CR));
	rprintfProgStrM("  ");
	rprintfu08(ax88796Read(BNRY));
	rprintfProgStrM("   ");
	rprintfu08(ax88796Read(PSTART));
	rprintfProgStrM("  ");
	rprintfu08(ax88796Read(PSTOP));
	rprintfProgStrM("  ");
	rprintfu08(ax88796Read(ISR));
	rprintfProgStrM("  ");
	rprintfu08(ax88796Read(TSR));
	rprintfProgStrM("  ");
	rprintfu08(ax88796Read(RSR));
	rprintfProgStrM("  ");
	rprintfu08(ax88796Read(MEMR));
	rprintfProgStrM("  ");
	rprintfu08(ax88796Read(TR));
	rprintfProgStrM("  ");
	rprintfu08(ax88796Read(GPI));
	rprintfCRLF();

	ax88796Write(CR,ax88796Read(CR)|PS0);

	rprintf("Page1: CR  PAR    CPR\r\n");
	rprintfProgStrM("       ");
	rprintfu08(ax88796Read(CR));
	rprintfProgStrM("  ");
	rprintfChar(ax88796Read(PAR0));
	rprintfChar(ax88796Read(PAR1));
	rprintfChar(ax88796Read(PAR2));
	rprintfChar(ax88796Read(PAR3));
	rprintfChar(ax88796Read(PAR4));
	rprintfChar(ax88796Read(PAR5));
	rprintfProgStrM(" ");
	rprintfu08(ax88796Read(CPR));
	
	ax88796Write(CR,ax88796Read(CR)&~PS0);

	delay_ms(25);
}

/*
unsigned char ax88796ReceiveEmpty(void)
{
	unsigned char temp;

	// read CPR from page 1
	ax88796Write(CR,0x62);
	temp = ax88796Read(CPR);
	
	// return to page 0
	ax88796Write(CR,0x22);
	
	return ( ax88796Read(BNRY) == temp );
	
}*/




