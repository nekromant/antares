/*! \file param.h \brief EEPROM Parameter Storage Library. */
//*****************************************************************************
//
// File Name	: 'param.h'
// Title		: EEPROM Parameter Storage Library
// Author		: Pascal Stang (c)2005
// Created		: 9/16/2005
// Revised		: 9/20/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
///	\ingroup driver_avr
///	\defgroup param EEPROM Parameter Storage Library (param.c)
///	\code #include "param.h" \endcode
///	\par Description
///		The "Param" library provide two simple functions for loading and
///		storing blocks of parameters out of and into internal EEPROM.
//
//	This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//*****************************************************************************
//@{

#ifndef PARAM_H
#define PARAM_H 


/*!	paramStore saves a block of data/parameters into EEPROM from RAM,
	and performs an 8-bit checksum, stored in EEPROM immediately after the data block.
	\param parameters	pointer to SRAM location where parameters are stored
	\param memaddr		pointer to EEPROM memory address where parameters should be written
	\param sizebytes	size of parameter block in bytes */
void paramStore(u08* parameters, u08* memaddr, u16 sizebytes);

/*!	paramLoad loads a block of data/parameters into RAM from EEPROM,
	and performs an 8-bit checksum to validate it.
	\param parameters	pointer to SRAM location into which parameters should be loaded
	\param memaddr		pointer to EEPROM memory address parameters are stored 
	\param sizebytes	size of parameter block in bytes
	\return				TRUE if load successful and checksum matched, FALSE otherwise */
u08 paramLoad(u08* parameters, u08* memaddr, u16 sizebytes);

#endif
//@}
