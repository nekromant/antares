/*! \file i2csw.h \brief Software I2C interface using port pins. */
//*****************************************************************************
//
// File Name	: 'i2csw.h'
// Title		: Software I2C interface using port pins
// Author		: Pascal Stang
// Created		: 11/22/2000
// Revised		: 5/2/2002
// Version		: 1.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
///	\ingroup driver_sw
/// \defgroup i2csw Software I2C Serial Interface Function Library (i2csw.c)
/// \code #include "i2csw.h" \endcode
/// \par Overview
///		This library provides a very simple bit-banged I2C serial interface.
/// The library supports MASTER mode send and receive of single or multiple
/// bytes.  Thanks to the standardization of the I2C protocol and register
/// access, the send and receive commands are everything you need to talk to
/// thousands of different I2C devices including: EEPROMS, Flash memory,
/// MP3 players, A/D and D/A converters, electronic potentiometers, etc.
///
/// Although some AVR processors have built-in hardware to help create an I2C
/// interface, this library does not require or use that hardware.
///
/// For general information about I2C, see the i2c library.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef I2CSW_H
#define I2CSW_H

#include "global.h"

// include project-dependent settings
#include "i2cswconf.h"

// defines and constants
#define READ		0x01	// I2C READ bit

// functions

// initialize I2C interface pins
void i2cInit(void);

// send I2C data to <device> register <sub>
void i2cSend(BYTE device, BYTE sub, BYTE length, BYTE *data);

// receive I2C data from <device> register <sub>
void i2cReceive(BYTE device, BYTE sub, BYTE length, BYTE *data);

#endif
