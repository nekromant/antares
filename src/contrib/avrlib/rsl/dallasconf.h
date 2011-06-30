/*! \file dallasconf.h \brief Dallas 1-Wire Bus Library Configuration. */
//*****************************************************************************
//
// File Name	: 'dallasconf.h'
// Title		: Dallas 1-Wire Bus Library Configuration
// Author		: Pascal Stang - Copyright (C) 2004
// Created		: 2004.09.10
// Revised		: 2004.10.10
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef DALLASCONF_H
#define DALLASCONF_H

// Select which general-purpose I/O pin
// will be used for driving the dallas bus
#define DALLAS_PORT					PORTE		// the output port
#define DALLAS_DDR					DDRE		// the DDR port
#define DALLAS_PORTIN				PINE		// the input port
#define DALLAS_PIN					7			// the pin number [0-7]

// Define the max number of Dallas devices which
// can be automatically discovered on the bus
#define DALLAS_MAX_DEVICES			20

#endif
