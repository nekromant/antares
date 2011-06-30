/*! \file spiflash.c \brief SPI Flash Memory Driver (M25Pxx/AT25Fxxx/etc). */
//*****************************************************************************
//
// File Name	: 'spiflash.c'
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

// system includes
#include "avr/io.h"
#include "global.h"

// library includes
#include "spi.h"
#include "spiflash.h"

#define SPIFLASH_CONFIG_CS		DDRB  |= (1<<0)
#define SPIFLASH_ASSERT_CS		PORTB &= (1<<0)
#define SPIFLASH_RELEASE_CS		PORTB |= (1<<0)

// functions
void spiflashInit(void)
{
	// initialize spi
	spiInit();
	// initialize chip select
	SPIFLASH_RELEASE_CS;
	SPIFLASH_CONFIG_CS;
}

unsigned short spiflashGetID(void)
{
	unsigned short id;

	SPIFLASH_ASSERT_CS;
	spiByte(0, SPIFLASH_CMD_RDID, 0);
	id  = spiByte(0, 0x00, 0)<<8;
	id |= spiByte(0, 0x00, 1);
	SPIFLASH_RELEASE_CS;

	return id;
}

void spiflashChipErase(void)
{
	// enable write
	SPIFLASH_ASSERT_CS;
	spiByte(0, SPIFLASH_CMD_WREN, 1);
	SPIFLASH_RELEASE_CS;

	// clock out dummy byte to waste time
	spiByte(0, 0x00, 1);

	// do chip erase
	SPIFLASH_ASSERT_CS;
	spiByte(0, SPIFLASH_CMD_CHIPERASE, 1);
	SPIFLASH_RELEASE_CS;

	// clock out dummy byte to waste time
	spiByte(0, 0x00, 1);

	// wait until write is done
	SPIFLASH_ASSERT_CS;
	spiByte(0, SPIFLASH_CMD_RDSR, 0);
	while(spiByte(0, 0x00, 0) & SPIFLASH_STATUS_BUSY);
	SPIFLASH_RELEASE_CS;
}

void spiflashRead(unsigned long addr, unsigned long nbytes, unsigned char *data)
{
	// begin read
	SPIFLASH_ASSERT_CS;
	// issue read command
	spiByte(0, SPIFLASH_CMD_READ, 0);
	// send address
	spiByte(0, addr>>16, 0);
	spiByte(0, addr>>8, 0);
	spiByte(0, addr>>0, 0);
	// read data
	while(nbytes--)
		*data++ = spiByte(0, 0x00, 0);
	// end read
	SPIFLASH_RELEASE_CS;
}

void spiflashWrite(unsigned long addr, unsigned long nbytes, unsigned char *data)
{
	unsigned int page;
	unsigned int i;
	unsigned int pagelen;

	// loop through pages to be programmed
	for(page=0; page<((nbytes+SPIFLASH_PAGESIZE-1)>>8); page++)
	{
		// program this page

		// enable write
		SPIFLASH_ASSERT_CS;
		spiByte(0, SPIFLASH_CMD_WREN, 1);
		SPIFLASH_RELEASE_CS;
		
		// clock out dummy byte to waste time
		spiByte(0, 0x00, 1);

		// begin write
		SPIFLASH_ASSERT_CS;
		// issue write command
		spiByte(0, SPIFLASH_CMD_PAGEPROG, 0);
		// send address
		spiByte(0, addr>>16, 0);
		spiByte(0, addr>>8, 0);
		spiByte(0, addr>>0, 0);
		// program exactly the number of bytes requested
		if( ((page<<8)+SPIFLASH_PAGESIZE) <= nbytes)
			pagelen = SPIFLASH_PAGESIZE;
		else
			pagelen = nbytes-(page<<8);
		// transfer data
		for(i=0; i<pagelen; i++)
			spiByte(0, *data++, 0);
		// end write
		SPIFLASH_RELEASE_CS;

		// clock out dummy byte to waste time
		spiByte(0, 0x00, 1);

		// wait until write is done
		SPIFLASH_ASSERT_CS;
		spiByte(0, SPIFLASH_CMD_RDSR, 0);
		while(spiByte(0, 0x00, 0) & SPIFLASH_STATUS_BUSY);
		SPIFLASH_RELEASE_CS;

		// clock out dummy byte to waste time
		spiByte(0, 0x00, 1);
	}
}
