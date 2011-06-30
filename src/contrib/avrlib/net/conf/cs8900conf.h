/*! \file cs8900conf.h \brief Crystal CS8900 Ethernet Interface Driver Configuration. */
//*****************************************************************************
//
// File Name	: 'cs8900conf.h'
// Title		: Crystal CS8900 Ethernet Interface Driver Configuration
// Author		: Pascal Stang
// Created		: 11/7/2004
// Revised		: 8/22/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
///	\ingroup network
//
// Description	: This driver provides initialization and transmit/receive
//		functions for the Crystal CS8900 10Mb Ethernet Controller and PHY.
//
//*****************************************************************************

#ifndef CS8900CONF_H
#define CS8900CONF_H

// This driver supports a CS8900 connected in memory-mapped or direct I/O mode.
//
#define GENERAL_IO				0
// Direct I/O mode assumes the NIC address, data, and control lines are
// connected directly to processor I/O pins.  The memory-bus accesses are
// software emulated.
//
//#define MEMORY_MAPPED			1 (*** MEMORY-MAPPED NOT YET IMPLEMENTED ***)
// Memory-mapped mode assumes that the NIC is connected the processor via
// the external memory bus, and that the NIC address space starts at the
// memory location CS8900_MEMORY_MAPPED_OFFSET.
//
// In either mode, a seperate I/O pins is required for control of the NIC's
// hardware RESET line.


// set the connection type used to communicate with the NIC
#define NIC_CONNECTION GENERAL_IO

#if NIC_CONNECTION != GENERAL_IO
	// NIC is memory-mapped starting at this address
	#define CS8900_MEMORY_MAPPED_OFFSET 0x8000
#else // NIC Interface through General I/O
	// CS8900 address port
	#define CS8900_ADDRESS_PORT        PORTF
	#define CS8900_ADDRESS_DDR         DDRF
	#define CS8900_ADDRESS_MASK        0x0F
	// CS8900 data port
	#define CS8900_DATA_PORT           PORTA
	#define CS8900_DATA_DDR            DDRA
	#define CS8900_DATA_PIN            PINA
	// CS8900 control port
	#define CS8900_CONTROL_PORT        PORTB
	#define CS8900_CONTROL_DDR         DDRB
	#define CS8900_CONTROL_READPIN     4
	#define CS8900_CONTROL_WRITEPIN    3
#endif

// CS8900 RESET pin
#define CS8900_RESET_PORT 	PORTB
#define CS8900_RESET_DDR 	DDRB
#define CS8900_RESET_PIN 	1

// MAC address for this interface
#ifdef ETHADDR0
#define CS8900_MAC0 ETHADDR0
#define CS8900_MAC1 ETHADDR1
#define CS8900_MAC2 ETHADDR2
#define CS8900_MAC3 ETHADDR3
#define CS8900_MAC4 ETHADDR4
#define CS8900_MAC5 ETHADDR5
#else
#define CS8900_MAC0 '0'
#define CS8900_MAC1 'F'
#define CS8900_MAC2 'F'
#define CS8900_MAC3 'I'
#define CS8900_MAC4 'C'
#define CS8900_MAC5 'E'
#endif

#endif
