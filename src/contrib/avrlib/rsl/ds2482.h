/*! \file ds2482.h \brief Dallas DS2482 I2C-to-Dallas1Wire Master Library. */
//*****************************************************************************
//
// File Name	: 'ds2482.h'
// Title		: Dallas DS2482 I2C-to-Dallas1Wire Master Library
// Author		: Pascal Stang - Copyright (C) 2004
// Created		: 2004.09.27
// Revised		: 2004.09.27
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

#ifndef DS2482_H
#define DS2482_H

#include "global.h"

// constants/macros/typdefs
#define DS2482_I2C_ADDR		0x30	//< Base I2C address of DS2482 devices

// DS2482 command defines
#define DS2482_CMD_DRST		0xF0	//< DS2482 Device Reset
#define DS2482_CMD_WCFG		0xD2	//< DS2482 Write Configuration
#define DS2482_CMD_CHSL		0xC3	//< DS2482 Channel Select
#define DS2482_CMD_SRP		0xE1	//< DS2482 Set Read Pointer
#define DS2482_CMD_1WRS		0xB4	//< DS2482 1-Wire Reset
#define DS2482_CMD_1WWB		0xA5	//< DS2482 1-Wire Write Byte
#define DS2482_CMD_1WRB		0x96	//< DS2482 1-Wire Read Byte
#define DS2482_CMD_1WSB		0x87	//< DS2482 1-Wire Single Bit
#define DS2482_CMD_1WT		0x78	//< DS2482 1-Wire Triplet

// DS2482 status register bit defines
#define DS2482_STATUS_1WB	0x01	//< DS2482 Status 1-Wire Busy
#define DS2482_STATUS_PPD	0x02	//< DS2482 Status Presence Pulse Detect
#define DS2482_STATUS_SD	0x04	//< DS2482 Status Short Detected
#define DS2482_STATUS_LL	0x08	//< DS2482 Status 1-Wire Logic Level
#define DS2482_STATUS_RST	0x10	//< DS2482 Status Device Reset
#define DS2482_STATUS_SBR	0x20	//< DS2482 Status Single Bit Result
#define DS2482_STATUS_TSB	0x40	//< DS2482 Status Triplet Second Bit
#define DS2482_STATUS_DIR	0x80	//< DS2482 Status Branch Direction Taken

// DS2482 configuration register bit defines
#define DS2482_CFG_APU		0x01	//< DS2482 Config Active Pull-Up
#define DS2482_CFG_PPM		0x02	//< DS2482 Config Presence Pulse Masking
#define DS2482_CFG_SPU		0x04	//< DS2482 Config Strong Pull-Up
#define DS2482_CFG_1WS		0x08	//< DS2482 Config 1-Wire Speed

// DS2482 channel selection code defines
#define DS2482_CH_IO0		0xF0	//< DS2482 Select Channel IO0
#define DS2482_CH_IO1		0xE1	//< DS2482 Select Channel IO1
#define DS2482_CH_IO2		0xD2	//< DS2482 Select Channel IO2
#define DS2482_CH_IO3		0xC3	//< DS2482 Select Channel IO3
#define DS2482_CH_IO4		0xB4	//< DS2482 Select Channel IO4
#define DS2482_CH_IO5		0xA5	//< DS2482 Select Channel IO5
#define DS2482_CH_IO6		0x96	//< DS2482 Select Channel IO6
#define DS2482_CH_IO7		0x87	//< DS2482 Select Channel IO7

// DS2482 read pointer code defines
#define DS2482_READPTR_SR	0xF0	//< DS2482 Status Register
#define DS2482_READPTR_RDR	0xE1	//< DS2482 Read Data Register
#define DS2482_READPTR_CSR	0xD2	//< DS2482 Channel Selection Register
#define DS2482_READPTR_CR	0xC3	//< DS2482 Configuration Register

// functions

//! Initialize the DS2482 chip
//	returns:
//	0 if successful
//	non-zero if unsuccessful (chip not present)
u08 ds2482Init(u08 i2cAddr);
u08 ds2482Reset(u08 i2cAddr);
u08 ds2482SendCmd(u08 cmd);
u08 ds2482SendCmdArg(u08 cmd, u08 arg);
u08 ds2482BusyWait(void);
u08 ds2482BusReset(void);
u08 ds2482BusTransferBit(u08 bit);
u08 ds2482BusTriplet(u08 dir);
u08 ds2482BusLevel(void);
void ds2482BusWriteByte(u08 data);
u08 ds2482BusReadByte(void);

#endif
