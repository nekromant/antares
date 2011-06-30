/*! \file ads7870.h \brief TI ADS7870 12-bit 8ch A/D Converter Driver Library. */
//*****************************************************************************
//
// File Name	: 'ads7870.h'
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
///	\ingroup driver_hw
/// \defgroup ads7870 TI ADS7870 SPI A/D Converter Driver (ads7870.c)
/// \code #include "ads7870.h" \endcode
/// \par Overview
///		This library provides high-level functions for accessing the Texas
///		Instruments ADS7870 I2C A/D Converter.
///
///		The basic specs of the ADS7870 are:
///		- Fast SPI interface (up to 20MHz)
///		- 12-bit results
///		- 8 single-ended or 4 differential input channels
///		- Programmable gain stage (1,2,4,5,8,10,16,20x gains)
///		- Software or hardware triggered conversion
///		- 4-bit auxiliary digital I/O lines (controlled via serial interface)
///		- Up to 50KHz conversion rate
///		- External reference or internal 2.5V, 2.048V, 1.15V reference
///		- NOTE: use pin-compatible ADS7871 for 14-bit results
//
//*****************************************************************************
//@{

#ifndef ADS7870_H
#define ADS7870_H

#include "global.h"

// constants/macros/typdefs
#define ADS7870_CS_PORT			PORTB
#define ADS7870_CS_DDR			DDRB
#define ADS7870_CS_PIN			PB0

// instruction bit defines
#define ADS7870_CONVERT			0x80

#define ADS7870_REG_READ		0x40
#define ADS7870_REG_WRITE		0x00
#define ADS7870_REG_16BIT		0x20

// register addresses
#define ADS7870_RESULTLO		0x00
#define ADS7870_RESULTHI		0x01
#define ADS7870_PGAVALID		0x02
#define ADS7870_ADCTRL			0x03
#define ADS7870_GAINMUX			0x04
#define ADS7870_DIGIOSTATE		0x05
#define ADS7870_DIGIOCTRL		0x06
#define ADS7870_REFOSC			0x07
#define ADS7870_SERIFCTRL		0x18
#define ADS7870_ID				0x1F

// register bit defines
#define ADS7870_RESULTLO_OVR	0x01

#define ADS7870_ADCTRL_BIN		0x20
#define ADS7870_ADCTRL_RMB1		0x08
#define ADS7870_ADCTRL_RMB0		0x04
#define ADS7870_ADCTRL_CFD1		0x02
#define ADS7870_ADCTRL_CFD0		0x01

#define ADS7870_GAINMUX_CNVBSY	0x80

#define ADS7870_REFOSC_OSCR		0x20
#define ADS7870_REFOSC_OSCE		0x10
#define ADS7870_REFOSC_REFE		0x08
#define ADS7870_REFOSC_BUFE		0x04
#define ADS7870_REFOSC_R2V		0x02
#define ADS7870_REFOSC_RBG		0x01

#define ADS7870_SERIFCTRL_LSB	0x01
#define ADS7870_SERIFCTRL_2W3W	0x02
#define ADS7870_SERIFCTRL_8051	0x04

#define ADS7870_ID_VALUE		0x01

// gain defines
#define ADS7870_GAIN_1X			0x00
#define ADS7870_GAIN_2X			0x10
#define ADS7870_GAIN_4X			0x20
#define ADS7870_GAIN_5X			0x30
#define ADS7870_GAIN_8X			0x40
#define ADS7870_GAIN_10X		0x50
#define ADS7870_GAIN_16X		0x60
#define ADS7870_GAIN_20X		0x70
// channel defines
#define ADS7870_CH_0_1_DIFF		0x00
#define ADS7870_CH_2_3_DIFF		0x01
#define ADS7870_CH_4_5_DIFF		0x02
#define ADS7870_CH_6_7_DIFF		0x03
#define ADS7870_CH_1_0_DIFF		0x04
#define ADS7870_CH_3_2_DIFF		0x05
#define ADS7870_CH_5_4_DIFF		0x06
#define ADS7870_CH_7_6_DIFF		0x07
#define ADS7870_CH_SINGLE_ENDED	0x08
#define ADS7870_CH_0			0x08
#define ADS7870_CH_1			0x09
#define ADS7870_CH_2			0x0A
#define ADS7870_CH_3			0x0B
#define ADS7870_CH_4			0x0C
#define ADS7870_CH_5			0x0D
#define ADS7870_CH_6			0x0E
#define ADS7870_CH_7			0x0F

// functions

//! Initialize the ADS7870 chip.
///	Returns:
///	TRUE if successful, 
///	FALSE if unsuccessful (chip not responding).
u08 ads7870Init(void);

//! Begin single-ended conversion on given logical channel#0-7, and return result.
/// \note Result is returned left-justified.
s16 ads7870Convert(u08 channel);

//! Begin differential conversion on given channel pair, and return result.
/// \note Result is returned left-justified.
s16 ads7870ConvertDiff(u08 channel);

//! Begin conversion on given raw channel#, and return result.
/// \note Result is returned left-justified.
s16 ads7870ConvertRaw(u08 channel);

//! Read value from ADS7870 register.
///
u08 ads7870ReadReg(u08 reg);

//! Write value into ADS7870 register.
///
void ads7870WriteReg(u08 reg, u08 value);



#endif
//@}
