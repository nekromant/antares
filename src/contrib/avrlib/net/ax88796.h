/*! \file ax88796.h \brief ASIX AX88796 Ethernet Interface Driver. */
//*****************************************************************************
//
// File Name	: 'ax88796.h'
// Title		: ASIX AX88796 Ethernet Interface Driver
// Author		: Pascal Stang
// Created		: 10/22/2002
// Revised		: 8/22/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
///	\ingroup network
///	\defgroup ax88796 ASIX AX88796 Ethernet Interface Driver (ax88796.c)
///	\code #include "net/ax88796.h" \endcode
///	\par Overview
///		This driver provides initialization and transmit/receive
///	functions for the ASIX AX88796 10/100Mb Ethernet Controller and PHY.
///
/// Based in part on code by Louis Beaudoin (www.embedded-creations.com).
/// Thanks to Adam Dunkels and Louis Beaudoin for providing the initial
/// structure in which to write this driver.
//
//*****************************************************************************
//@{

#ifndef AX88796_H
#define AX88796_H

#include "global.h"

#define nop()	asm volatile ("nop")

// AX88796/NE2000 Control Register Offsets
// Page 0 - Read/Write
#define CR		 	0x00	// Command Register
#define PSTART		0x01	// Page Start Register
#define PSTOP		0x02	// Page Stop Register
#define BNRY		0x03	// Boundary Pointer
#define RDMAPORT  	0x10	// DMA Data Port
#define MEMR		0x14	// MII/EEPROM Access Register
#define TR			0x15	// Test Register
#define SPP_DPR    	0x18	// Standard Printer Port Data
#define SSP_SPR		0x19	// Standard Printer Port Status
#define SSP_CPR		0x1A	// Standard Printer Port Control
// Page 0 - Read
#define TSR			0x04	// Transmit Status Register
#define NCR			0x05	// Number of Collisions Register
#define ISR			0x07	// Interrupt Status Register
#define CRDA0		0x08	// Current Remote DMA Address 0
#define CRDA1		0x09	// Current Remote DMA Address 1
#define RSR			0x0C	// Receive Status Register
#define CNTR0		0x0D
#define CNTR1		0x0E
#define CNTR2		0x0F
#define GPI			0x17	// General-Purpose Input
#define RSTPORT		0x1F	// Reset
// Page 0 - Write
#define TPSR		0x04	// Transmit Page Start Address
#define TBCR0		0x05	// Transmit Byte Count Register 0
#define TBCR1		0x06	// Transmit Byte Count Register 1
#define RSAR0		0x08	// Remote Start Address Register 0
#define RSAR1		0x09	// Remote Start Address Register 1
#define RBCR0		0x0A	// Remote Byte Count 0
#define RBCR1		0x0B	// Remote Byte Count 1
#define RCR			0x0C	// Receive Config Register
#define TCR			0x0D	// Transmit Config Register
#define DCR			0x0E	// Data Config Register
#define IMR			0x0F	// Interrupt Mask Register
#define GPOC		0x17	// General-Purpose Output Control
// Page 1 - Read/Write
#define PAR0      	0x01	// Physical Address Register 0
#define PAR1      	0x02	// Physical Address Register 0
#define PAR2      	0x03	// Physical Address Register 0
#define PAR3      	0x04	// Physical Address Register 0
#define PAR4      	0x05	// Physical Address Register 0
#define PAR5      	0x06	// Physical Address Register 0
#define CURR		0x07	// Page 1
#define CPR			0x07	// Current Page Register

// AX88796 CR Register Bit Definitions
#define  PS1		0x80 
#define  PS0		0x40 
#define  RD2		0x20 
#define  RD1		0x10 
#define  RD0		0x08 
#define  TXP		0x04 
#define  START		0x02 
#define  STOP		0x01 
// AX88796 RCR Register Bit Definitions
#define  INTT		0x40 
#define  MON		0x20 
#define  PRO		0x10 
#define  AM			0x08 
#define  AB			0x04 
#define  AR			0x02 
#define  SEP		0x01 
// AX88796 ISR Register Bit Definitions
#define  RST		0x80
#define  RDC		0x40
#define  OVW		0x10
#define  RXE		0x08
#define  TXE		0x04
#define  PTX		0x02
#define  PRX		0x01
// AX88796 TEST Register Bit Definitions
#define  AUTOD		0x01 
#define  RST_B		0x02
#define  RST_10B	0x04
#define  RST_TXB	0x08
// AX88796 GPOC Register Bit Definitions
#define  GPO0		0x01
#define	 MPSEL		0x10
#define  MPSET		0x20
#define  PPDSET		0x40
// AX88796 MEMR Register Bit Definitions
#define  MDC		0x01
#define  MDIR		0x02
#define  MDI		0x04
#define  MDO		0x08
#define  EECS		0x10
#define  EEI		0x20
#define  EEO		0x40
#define  EECLK		0x80
// AX88796 GPI Register Bit Definitions
#define  GPI2		0x40
#define  GPI1		0x20
#define  GPI0		0x10
#define  I_SPD		0x04
#define  I_DPX		0x02
#define  I_LINK		0x01
// AX88796 TCR Register Bit Definitions
#define  FDU		0x80	// full duplex
#define  PD			0x40	// pad disable
#define  RLO		0x20	// retry of late collisions
#define  LB1		0x04	// loopback 1
#define  LB0		0x02	// loopback 0
#define  CRC		0x01	// generate CRC

// AX88796 Initial Register Values
// RCR : INT trigger active high and Accept Broadcast ENET packets
#define RCR_INIT		(INTT | AB)
#define DCR_INIT		0x00   // was 0x58 for realtek RTL8019
// TCR : default transmit operation - CRC is generated
#define TCR_INIT		0x00
// IMR : interrupt enabled for receive and overrun events
#define IMR_INIT		0x11    // PRX and OVW interrupt enabled
// buffer boundaries
//	transmit has 6 256-byte pages
//	receive has 26 256-byte pages
//	entire available packet buffer space is allocated
#define TXSTART_INIT   	0x40
#define RXSTART_INIT   	0x46
#define RXSTOP_INIT    	0x60

// Ethernet constants
#define ETHERNET_MIN_PACKET_LENGTH	0x3C
//#define ETHERNET_HEADER_LENGTH		0x0E

// offsets into ax88796 ethernet packet header
#define  PKTHEADER_STATUS		0x00	// packet status
#define  PKTHEADER_NEXTPAGE		0x01	// next buffer page
#define	 PKTHEADER_PKTLENL		0x02	// packet length low
#define	 PKTHEADER_PKTLENH		0x03	// packet length high


// functions
#include "nic.h"

// setup ports for I/O
void ax88796SetupPorts(void);

// read ax88796 register
u08 ax88796Read(u08 address);

// write ax88796 register
void ax88796Write(u08 address, u08 data);

// initialize the ethernet interface for transmit/receive
void ax88796Init(void);

// packet transmit functions
void ax88796BeginPacketSend(unsigned int packetLength);
void ax88796SendPacketData(unsigned char * localBuffer, unsigned int length);
void ax88796EndPacketSend(void);

// packet receive functions
unsigned int ax88796BeginPacketRetreive(void);
void ax88796RetreivePacketData(unsigned char *localBuffer, unsigned int length);
void ax88796EndPacketRetreive(void);

// Processes AX88796 interrupts.
// Currently, this function looks only for a receive overflow condition.
// The function need not be called in response to an interrupt,
// but can be executed just before checking the receive buffer for incoming packets.
void ax88796ProcessInterrupt(void);

// execute procedure for recovering from a receive overflow
// this should be done when the receive memory fills up with packets
void ax88796ReceiveOverflowRecover(void);

// Write MII Registers
void ax88796WriteMii(unsigned char phyad,unsigned char regad,unsigned int mii_data);
// Read MII Registers
unsigned int ax88796ReadMii(unsigned char phyad,unsigned char regad);

// formatted print of all important AX88796 registers
void ax88796RegDump(void);

#endif
//@}
