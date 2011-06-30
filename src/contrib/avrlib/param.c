/*! \file param.c \brief EEPROM Parameter Storage Library. */
//*****************************************************************************
//
// File Name	: 'param.c'
// Title		: EEPROM Parameter Storage Library
// Author		: Pascal Stang (c)2005
// Created		: 9/16/2005
// Revised		: 9/20/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
//*****************************************************************************

#include <avr/eeprom.h>

#include "global.h"

u08 paramLoad(u08* parameters, u08* memaddr, u16 sizebytes)
{
	u16 i;
	u08 checksum_stored=0;
	u08 checksum=0;

	// load parameters
	eeprom_read_block(parameters, memaddr, sizebytes);
	// load checksum
	eeprom_read_block(&checksum_stored, memaddr+sizebytes, sizeof(u08));

	// calculate own checksum
	for(i=0;i<sizebytes;i++)
		checksum += parameters[i];
	checksum = ~checksum;
	
	if(checksum == checksum_stored)
		return TRUE;
	else
		return FALSE;
}

void paramStore(u08* parameters, u08* memaddr, u16 sizebytes)
{
	u16 i;
	u08 checksum=0;

	// calculate checksum
	for(i=0;i<sizebytes;i++)
		checksum += parameters[i];
	checksum = ~checksum;

	// store parameters
	eeprom_write_block(parameters, memaddr, sizebytes);
	// store checksum
	eeprom_write_block(&checksum, memaddr+sizebytes, sizeof(u08));
}

