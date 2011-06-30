/*! \file ax88796conf.h \brief ASIX AX88796 Ethernet Interface Driver Configuration. */
//*****************************************************************************
//
// File Name	: 'ax88796conf.h'
// Title		: ASIX AX88796 Ethernet Interface Driver Configuration
// Author		: Pascal Stang
// Created		: 10/22/2002
// Revised		: 8/22/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// Description	: This driver provides initialization and transmit/receive
//		functions for the ASIX AX88796 10/100Mb Ethernet Controller and PHY.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef AX88796CONF_H
#define AX88796CONF_H

// This driver supports an AX88796 connected in memory-mapped or direct I/O mode.
//
#define GENERAL_IO				0
// Direct I/O mode assumes the AX88796 address, data, and control lines are
// connected directly to processor I/O pins.  The memory-bus accesses are
// software emulated.
//
#define MEMORY_MAPPED			1
// Memory-mapped mode assumes that the AX88796 is connected the processor via
// the external memory bus, and that the AX88796 address space starts at the
// memory location AX88796_MEMORY_MAPPED_OFFSET.
//
// In either mode, a seperate I/O pins is required for control of the AX88796's
// hardware RESET line.

// set the connection type used to communicate with the NIC
#define NIC_CONNECTION GENERAL_IO


#if NIC_CONNECTION != GENERAL_IO
	// NIC is memory-mapped starting at this address
	#define AX88796_MEMORY_MAPPED_OFFSET 0x2000
#else // NIC Interface through General I/O
	// AX88796 address port
	#define AX88796_ADDRESS_PORT        PORTB
	#define AX88796_ADDRESS_DDR         DDRB
	#define AX88796_ADDRESS_MASK		0x1F
	// AX88796 data port
	#define AX88796_DATA_PORT           PORTA
	#define AX88796_DATA_DDR            DDRA
	#define AX88796_DATA_PIN            PINA
	// AX88796 control port
	#define AX88796_CONTROL_PORT        PORTD
	#define AX88796_CONTROL_DDR         DDRD
	#define AX88796_CONTROL_READPIN     PD5
	#define AX88796_CONTROL_WRITEPIN    PD4
#endif

// AX88796 RESET pin
#define AX88796_RESET_PORT 	PORTD
#define AX88796_RESET_DDR 	DDRD
#define AX88796_RESET_PIN 	PD6

// MAC address for this interface
#ifdef ETHADDR0
#define AX88796_MAC0 ETHADDR0
#define AX88796_MAC1 ETHADDR1
#define AX88796_MAC2 ETHADDR2
#define AX88796_MAC3 ETHADDR3
#define AX88796_MAC4 ETHADDR4
#define AX88796_MAC5 ETHADDR5
#else
#define AX88796_MAC0 '0'
#define AX88796_MAC1 'F'
#define AX88796_MAC2 'F'
#define AX88796_MAC3 'I'
#define AX88796_MAC4 'C'
#define AX88796_MAC5 'E'
#endif

#endif
