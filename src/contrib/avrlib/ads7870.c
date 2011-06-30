/*! \file ads7870.c \brief TI ADS7870 12-bit 8ch A/D Converter Driver Library. */
//*****************************************************************************
//
// File Name	: 'ads7870.c'
// Title		: TI ADS7870 12-bit 8ch A/D Converter Driver Library
// Author		: Pascal Stang - Copyright (C) 2005
// Created		: 2005.07.19
// Revised		: 2005.07.21
// Version		: 0.1
// Target MCU	: Atmel AVR Series
// Editor Tabs	: 4
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include <avr/io.h>
#include <avr/interrupt.h>

#include "global.h"
#include "spi.h"
#include "ads7870.h"

// global variables

// Functions
u08 ads7870Init(void)
{
	// initialize spi interface
	spiInit();
	// switch to f/4 bitrate
	cbi(SPCR, SPR0);
	cbi(SPCR, SPR1);
	//sbi(SPSR, SPI2X);

	// setup chip select
	sbi(ADS7870_CS_PORT, ADS7870_CS_PIN);
	sbi(ADS7870_CS_DDR, ADS7870_CS_PIN);

	// check ID register
	if(ads7870ReadReg(ADS7870_ID) != ADS7870_ID_VALUE)
		return 0;

	// setup reference and buffer
	ads7870WriteReg(ADS7870_REFOSC, ADS7870_REFOSC_OSCE | ADS7870_REFOSC_REFE | ADS7870_REFOSC_BUFE);

	// return success
	return 1;
}

s16 ads7870Convert(u08 channel)
{
	// set single-ended channel bit
	channel |= ADS7870_CH_SINGLE_ENDED;
	// do conversion
	return ads7870ConvertRaw(channel);
}

s16 ads7870ConvertDiff(u08 channel)
{
	// clear single-ended channel bit
	channel &= ~ADS7870_CH_SINGLE_ENDED;
	// do conversion
	return ads7870ConvertRaw(channel);
}

s16 ads7870ConvertRaw(u08 channel)
{
	s16 result;
	// assert chip select
	cbi(ADS7870_CS_PORT, ADS7870_CS_PIN);
	// start conversion
	spiTransferByte(ADS7870_CONVERT | channel);
	// wait for completion
	while( ads7870ReadReg(ADS7870_GAINMUX) & ADS7870_GAINMUX_CNVBSY);
	// assert chip select
	cbi(ADS7870_CS_PORT, ADS7870_CS_PIN);
	// read result
	spiTransferByte(ADS7870_REG_READ | ADS7870_REG_16BIT | ADS7870_RESULTHI);
	result  = spiTransferByte(0x00)<<8;
	result |= spiTransferByte(0x00);
	// release chip select
	sbi(ADS7870_CS_PORT, ADS7870_CS_PIN);
	// return result
	return result;
}

u08 ads7870ReadReg(u08 reg)
{
	u08 data;
	// assert chip select
	cbi(ADS7870_CS_PORT, ADS7870_CS_PIN);
	// issue reg read command
	spiTransferByte(ADS7870_REG_READ | reg);
	// read data
	data = spiTransferByte(0x00);
	// release chip select
	sbi(ADS7870_CS_PORT, ADS7870_CS_PIN);
	// return data
	return data;
}

void ads7870WriteReg(u08 reg, u08 value)
{
	// assert chip select
	cbi(ADS7870_CS_PORT, ADS7870_CS_PIN);
	// issue reg write command
	spiTransferByte(ADS7870_REG_WRITE | reg);
	// write data
	spiTransferByte(value);
	// release chip select
	sbi(ADS7870_CS_PORT, ADS7870_CS_PIN);
}

