/*! \file osc.h \brief Open Sound Control (OSC) client functions. */
//*****************************************************************************
//
// File Name	: 'osc.h'
// Title		: Open Sound Control (OSC) client functions
// Author		: Pascal Stang - Copyright (C) 2002
// Created		: 10/30/2002
// Revised		: 11/4/2002
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// Description	: This code implements a subset of the OSC protocol and
//		messages.  It is meant to be used with the OSC extension for the visual
//		programming and data-processing package Pure-Data (or PD).  Note that
//		this code sends OSC messages over serial RS-232, not a network.  You
//		must use these functions with a suitable OSC SERIAL server (receiver)
//		on a host machine.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef OSC_H
#define OSC_H

#include "global.h"

// OSC serial packet structure:
// [HEADER][LENGTH][OSC MESSAGE][CHECKSUM] 
// where
// [HEADER] is the byte 0xBE
// [LENGTH] is the number of bytes in OSC MESSGAGE (must be a multiple of four)
// [OSC MESSAGE] is defined by the OSC specification
// [CHECKSUM] is the 8-bit sum of bytes in the OSC MESSAGE

// defines
#define OSC_HEADER	0xBE

// function prototypes

//! Initializes the OSC function library
void oscInit(void);

//! sends an OSC message without any arguments (message with OSC address only)
void oscSendMessage(char *address);

//! sends an OSC message with one 32-bit integer argument
void oscSendMessageInt(char *address, u32 arg);

//! sends an OSC message with two 32-bit integer arguments
void oscSendMessageIntInt(char *address, u32 arg, u32 arg2);

//! sends an OSC message with a null-terminated string argument
void oscSendMessageString(char *address, char *arg);

// private functions
void oscWriteHeader(char *address, u08 arglen);	///< writes the OSC header+length
void oscWriteString(char *address);				///< writes the OSC address or strings
void oscWriteChecksum(void);					///< calculates and writes the OSC checksum

#endif
