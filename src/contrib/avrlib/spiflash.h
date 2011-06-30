/*! \file spiflash.h \brief SPI Flash Memory Driver (M25Pxx/AT25Fxxx/etc). */
//*****************************************************************************
//
// File Name	: 'spiflash.h'
// Title		: SPI Flash Memory Driver (M25Pxx/AT25Fxxx/etc)
// Author		: Pascal Stang - Copyright (C) 2006
// Created		: 2006-04-15
// Revised		: 2006-07-02
// Version		: 0.1
// Target MCU	: AVR processors
// Editor Tabs	: 4
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
//*****************************************************************************

#ifndef AVRLIB_SPIFLASH_H
#define AVRLIB_SPIFLASH_H

#include "global.h"

// Compatible with:
// - ST M25Pxx devices
// - Atmel AT25Fxxx devices
// - many more

// device commands
#define SPIFLASH_CMD_WREN		0x06	// write enable
#define SPIFLASH_CMD_WRDI		0x04	// write disable
#define SPIFLASH_CMD_RDID		0x9F	// read ID register
#define SPIFLASH_CMD_RDSR		0x05	// read status register
#define SPIFLASH_CMD_WRSR		0x01	// write status register
#define SPIFLASH_CMD_READ		0x03	// read
#define SPIFLASH_CMD_FASTREAD	0x0B	// high-speed read
#define SPIFLASH_CMD_PAGEPROG	0x02	// page program
#define SPIFLASH_CMD_SECTERASE	0xD8	// sector erase
#define SPIFLASH_CMD_CHIPERASE	0xC7	// chip erase

// status register bits
#define SPIFLASH_STATUS_BUSY	0x01	// busy, write in progress
#define SPIFLASH_STATUS_WEN		0x02	// write enable
#define SPIFLASH_STATUS_BP0		0x04	// block protect 0
#define SPIFLASH_STATUS_BP1		0x08	// block protect 1
#define SPIFLASH_STATUS_BP2		0x10	// block protect 2
#define SPIFLASH_STATUS_WPEN	0x80	// write protect enabled

// device constants
#define SPIFLASH_PAGESIZE		256		// 256 bytes/page

// functions

// Initializes spiflash for operation
void spiflashInit(void);

// Get spiflash ID
// NOTE: supported only on some flash memories
unsigned short spiflashGetID(void);

// Erase entire flash chip
void spiflashChipErase(void);

// Read flash memory
// - addr may be any value
// - nbytes may be any value
void spiflashRead(unsigned long addr, unsigned long nbytes, unsigned char *data);

// Write flash memory (automatic handling of page writes)
// - memory must be previously erased
// - addr MUST start at a page break
//	OR
// - nbytes must be small enough such that page boundary is not crossed
void spiflashWrite(unsigned long addr, unsigned long nbytes, unsigned char *data);

#endif
