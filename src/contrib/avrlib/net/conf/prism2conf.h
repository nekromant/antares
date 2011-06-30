/*! \file prism2conf.h \brief Prism2 802.11b Wireless-LAN Interface Driver Configuration. */
//*****************************************************************************
//
// File Name	: 'prism2conf.h'
// Title		: Prism2 802.11b Wireless-LAN Interface Driver Configuration
// Author		: Pascal Stang
// Created		: 12/27/2004
// Revised		: 1/7/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
///	\ingroup network
//
// Description	: This is the configuration file for the Prism2 802.11b
//		Wireless-LAN Controller Driver.
//
//*****************************************************************************

#ifndef PRISM2CONF_H
#define PRISM2CONF_H

// This driver supports a PRISM2 NIC connected in memory-mapped or direct I/O mode.
//
#define GENERAL_IO				0
// Direct I/O mode assumes the NIC address, data, and control lines are
// connected directly to processor I/O pins.  The memory-bus accesses are
// software emulated.
//
#define MEMORY_MAPPED			1
// *** MEMORY-MAPPED NOT YET IMPLEMENTED ***
// Memory-mapped mode assumes that the NIC is connected the processor via
// the external memory bus, and that the NIC address space starts at the
// memory location PRISM2_MEMORY_MAPPED_OFFSET.
//
// In either mode, a seperate I/O pins is required for control of the NIC's
// hardware RESET line.

// set the connection type used to communicate with the NIC
#define NIC_CONNECTION GENERAL_IO

#if NIC_CONNECTION != GENERAL_IO
	// NIC is memory-mapped starting at this address
	#define PRISM2_MEMORY_MAPPED_OFFSET 0x8000
#else // NIC Interface through General I/O
	// PRISM2 address port
	#define PRISM2_ADDRESS_PORT			PORTA
	#define PRISM2_ADDRESS_DDR			DDRA
	#define PRISM2_ADDRESS_MASK			0xFF
	// PRISM2 high address port
	#define PRISM2_HADDRESS_PORT		PORTF
	#define PRISM2_HADDRESS_DDR			DDRF
	#define PRISM2_HADDRESS_MASK		0x07
	// PRISM2 data port
	#define PRISM2_DATA_PORT			PORTC
	#define PRISM2_DATA_DDR				DDRC
	#define PRISM2_DATA_PIN				PINC
	// PRISM2 control port
	#define PRISM2_CONTROL_PORT			PORTG
	#define PRISM2_CONTROL_DDR			DDRG
	#define PRISM2_CONTROL_IORD			1
	#define PRISM2_CONTROL_IOWR			2
	#define PRISM2_CONTROL_MEMRD		0
	#define PRISM2_CONTROL_MEMWR		3
	// Set PRISM2 memory and I/O bus access delay
	// NOTE: PRISM2 cards may not respond correctly if access time is too short or too long
	// Typically good settings: MEM = ~12us,  I/O = ~1us
	#define PRISM2_MEM_ACCESS_DELAY			delay_us(12)
	#define PRISM2_IO_ACCESS_DELAY			{ nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop(); }
	//#define PRISM2_IO_ACCESS_DELAY			{ nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop(); }
#endif

// PRISM2 RESET pin
#define PRISM2_RESET_PORT 	PORTG
#define PRISM2_RESET_DDR 	DDRG
#define PRISM2_RESET_PIN 	4



// MAC address for this interface
#ifdef ETHADDR0
#define PRISM2_MAC0 ETHADDR0
#define PRISM2_MAC1 ETHADDR1
#define PRISM2_MAC2 ETHADDR2
#define PRISM2_MAC3 ETHADDR3
#define PRISM2_MAC4 ETHADDR4
#define PRISM2_MAC5 ETHADDR5
#else
#define PRISM2_MAC0 '0'
#define PRISM2_MAC1 'F'
#define PRISM2_MAC2 'F'
#define PRISM2_MAC3 'I'
#define PRISM2_MAC4 'C'
#define PRISM2_MAC5 'E'
#endif

#endif
