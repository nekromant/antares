/*! \file rtl8019.h \brief Realtek RTL8019AS Ethernet Interface Driver. */
//*****************************************************************************
//
// File Name	: 'rtl8019.h'
// Title		: Realtek RTL8019AS Ethernet Interface Driver
// Author		: Pascal Stang
// Created		: 7/6/2004
// Revised		: 8/22/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
///	\ingroup network
///	\defgroup rtl8019 Realtek RTL8019AS Ethernet Interface Driver (rtl8019.c)
///	\code #include "net/rtl8019.h" \endcode
///	\par Overview
///		This driver provides initialization and transmit/receive
///	functions for the Realtek RTL8019AS 10Mb Ethernet Controller and PHY.
///
/// Based in part on code by Louis Beaudoin (www.embedded-creations.com).
/// Thanks to Adam Dunkels and Louis Beaudoin for providing the initial
/// structure in which to write this driver.
//
//*****************************************************************************
//@{

#ifndef RTL8019_H
#define RTL8019_H

#define nop()	asm volatile ("nop")

// RTL8019 Control Register Offsets
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
#define PAR1      	0x02	// Physical Address Register 1
#define PAR2      	0x03	// Physical Address Register 2
#define PAR3      	0x04	// Physical Address Register 3
#define PAR4      	0x05	// Physical Address Register 4
#define PAR5      	0x06	// Physical Address Register 5
#define CURR		0x07	// Page 1
#define CPR			0x07	// Current Page Register

#define RTL_EECR	0x01    // page 3
#define CR9346    	0x01    // Page 3
#define CONFIG2     0x05    // page 3
#define CONFIG3     0x06    // page 3

// RTL8019/NE2000 CR Register Bit Definitions
#define  PS1		0x80 
#define  PS0		0x40 
#define  RD2		0x20 
#define  RD1		0x10 
#define  RD0		0x08 
#define  TXP		0x04 
#define  START		0x02 
#define  STOP		0x01 
// RTL8019/NE2000 ISR Register Bit Definitions
#define  RST		0x80
#define  RDC		0x40
#define  OVW		0x10
#define  RXE		0x08
#define  TXE		0x04
#define  PTX		0x02
#define  PRX		0x01
// RTL8019/NE2000 RCR Register Bit Definitions
#define  MON		0x20
#define  PRO		0x10
#define  AM			0x08
#define  AB			0x04
#define  AR			0x02
#define  SEP		0x01
// RTL8019/NE2000 TCR Register Bit Definitions
#define  FDU		0x80	// full duplex
#define  PD			0x40	// pad disable
#define  RLO		0x20	// retry of late collisions
#define  LB1		0x04	// loopback 1
#define  LB0		0x02	// loopback 0
#define  CRC		0x01	// generate CRC
// RTL8019 EECR Register Bit Definitions
#define  EEM1		0x80
#define  EEM0		0x40
#define  EECS		0x08
#define  EESK		0x04
#define  EEDI		0x02
#define  EEDO		0x01


// RTL8019 Initial Register Values
// RCR : INT trigger active high and Accept Broadcast ENET packets
#define RCR_INIT		(AB)
#define DCR_INIT		0x58	// FIFO thrsh. 8bits, 8bit DMA transfer
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
void rtl8019SetupPorts(void);

// read ax88796 register
unsigned char rtl8019Read(unsigned char address);

// write ax88796 register
void rtl8019Write(unsigned char address, unsigned char data);

// initialize the ethernet interface for transmit/receive
void rtl8019Init(void);

// packet transmit functions
void rtl8019BeginPacketSend(unsigned int packetLength);
void rtl8019SendPacketData(unsigned char * localBuffer, unsigned int length);
void rtl8019EndPacketSend(void);

// packet receive functions
unsigned int rtl8019BeginPacketRetreive(void);
void rtl8019RetreivePacketData(unsigned char * localBuffer, unsigned int length);
void rtl8019EndPacketRetreive(void);

// Processes RTL8019 interrupts.
// Currently, this function looks only for a receive overflow condition.
// The function need not be called in response to an interrupt,
// but can be executed just before checking the receive buffer for incoming packets.
void rtl8019ProcessInterrupt(void);

// execute procedure for recovering from a receive overflow
// this should be done when the receive memory fills up with packets
void rtl8019ReceiveOverflowRecover(void);

// formatted print of all important RTL8019 registers
void rtl8019RegDump(void);

#endif
//@}
