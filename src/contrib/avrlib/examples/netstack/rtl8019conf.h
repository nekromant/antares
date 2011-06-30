/*! \file rtl8019conf.h \brief Realtek RTL8019AS Ethernet Interface Driver Configuration. */
//*****************************************************************************
//
// File Name	: 'rtl8019conf.h'
// Title		: Realtek RTL8019AS Ethernet Interface Driver Configuration
// Author		: Pascal Stang
// Created		: 10/5/2004
// Revised		: 8/22/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// Description	: This driver provides initialization and transmit/receive
//		functions for the RTL8019AS 10Mb Ethernet Controller and PHY.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef RTL8019CONF_H
#define RTL8019CONF_H

// This driver supports an RTL8019 connected in memory-mapped or direct I/O mode.
//
#define GENERAL_IO				0
// Direct I/O mode assumes the NIC address, data, and control lines are
// connected directly to processor I/O pins.  The memory-bus accesses are
// software emulated.
//
#define MEMORY_MAPPED			1
// Memory-mapped mode assumes that the NIC is connected the processor via
// the external memory bus, and that the NIC address space starts at the
// memory location RTL8019_MEMORY_MAPPED_OFFSET.
//
// In either mode, a seperate I/O pins is required for control of the NIC's
// hardware RESET line.

// set the connection type used to communicate with the NIC
#define NIC_CONNECTION GENERAL_IO


#if NIC_CONNECTION != GENERAL_IO
	// NIC is memory-mapped starting at this address
	#define RTL8019_MEMORY_MAPPED_OFFSET 0x8000
#else // NIC Interface through General I/O
	// RTL8019 address port
	#define RTL8019_ADDRESS_PORT        PORTB
	#define RTL8019_ADDRESS_DDR         DDRB
	#define RTL8019_ADDRESS_MASK        0x1F
	// RTL8019 data port
	#define RTL8019_DATA_PORT           PORTA
	#define RTL8019_DATA_DDR            DDRA
	#define RTL8019_DATA_PIN            PINA
	// RTL8019 control port
	#define RTL8019_CONTROL_PORT        PORTD
	#define RTL8019_CONTROL_DDR         DDRD
	#define RTL8019_CONTROL_READPIN     6
	#define RTL8019_CONTROL_WRITEPIN    7
#endif

// RTL8019 RESET pin
#define RTL8019_RESET_PORT 	PORTD
#define RTL8019_RESET_DDR 	DDRD
#define RTL8019_RESET_PIN 	4

// MAC address for this interface
#ifdef ETHADDR0
#define RTL8019_MAC0 ETHADDR0
#define RTL8019_MAC1 ETHADDR1
#define RTL8019_MAC2 ETHADDR2
#define RTL8019_MAC3 ETHADDR3
#define RTL8019_MAC4 ETHADDR4
#define RTL8019_MAC5 ETHADDR5
#else
#define RTL8019_MAC0 '0'
#define RTL8019_MAC1 'F'
#define RTL8019_MAC2 'F'
#define RTL8019_MAC3 'I'
#define RTL8019_MAC4 'C'
#define RTL8019_MAC5 'E'
#endif

#endif
