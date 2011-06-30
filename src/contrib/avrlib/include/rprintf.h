/*! \file rprintf.h \brief printf routine and associated routines. */
//****************************************************************************
//
// File Name	: 'rprintf.h'
// Title		: printf routine and associated routines
// Author		: Pascal Stang - Copyright (C) 2000-2002
// Created		: 2000.12.26
// Revised		: 2003.5.1
// Version		: 1.0
// Target MCU	: Atmel AVR series and other targets
// Editor Tabs	: 4
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
///	\ingroup general
/// \defgroup rprintf printf() Function Library (rprintf.c)
/// \code #include "rprintf.h" \endcode
/// \par Overview
///		The rprintf function library provides a simplified (reduced) version of
///		the common C printf() function.  See the code files for details about
///		which printf features are supported.  Also in this library are a
///		variety of functions for fast printing of certain common data types
///		(variable types).  Functions include print string from RAM, print
///		string from ROM, print string snippet, print hex byte/short/long, and
///		a custom-formatted number print, as well as an optional floating-point
///		print routine.
///
/// \note	All output from the rprintf library can be directed to any device
///		or software which accepts characters.  This means that rprintf output
///		can be sent to the UART (serial port) or can be used with the LCD
///		display libraries to print formatted text on the screen.
//
//****************************************************************************
//@{

#ifndef RPRINTF_H
#define RPRINTF_H

// needed for use of PSTR below
#include <avr/pgmspace.h>

// configuration
// defining RPRINTF_SIMPLE will compile a smaller, simpler, and faster printf() function
// defining RPRINTF_COMPLEX will compile a larger, more capable, and slower printf() function
#ifndef RPRINTF_COMPLEX
	#define RPRINTF_SIMPLE
#endif

// Define RPRINTF_FLOAT to enable the floating-point printf function: rprintfFloat()
// (adds +4600bytes or 2.2Kwords of code)

// defines/constants
#define STRING_IN_RAM	0
#define STRING_IN_ROM	1

// make a putchar for those that are used to using it
//#define putchar(c)	rprintfChar(c);

// functions

//! Initializes the rprintf library for an output stream.
/// You must call this initializer once before using any other rprintf function.
/// The argument must be a character stream output function.
void rprintfInit(void (*putchar_func)(unsigned char c));

//! prints a single character to the current output device
void rprintfChar(unsigned char c);

//! prints a null-terminated string stored in RAM
void rprintfStr(char str[]);

//! Prints a section of a string stored in RAM.
/// Begins printing at position indicated by <start>,
/// and prints number of characters indicated by <len>.
void rprintfStrLen(char str[], unsigned int start, unsigned int len);

//! prints a string stored in program rom
/// \note This function does not actually store your string in
/// program rom, but merely reads it assuming you stored it properly.
void rprintfProgStr(const prog_char str[]);

//! Using the function rprintfProgStrM(...) automatically causes 
/// your string to be stored in ROM, thereby not wasting precious RAM.
/// Example usage:
/// \code
/// rprintfProgStrM("Hello, this string is stored in program rom");
/// \endcode
#define rprintfProgStrM(string)			(rprintfProgStr(PSTR(string)))

//! Prints a carriage-return and line-feed.
/// Useful when printing to serial ports/terminals.
void rprintfCRLF(void);

// Prints the number contained in "data" in hex format
// u04,u08,u16,and u32 functions handle 4,8,16,or 32 bits respectively
void rprintfu04(unsigned char data);	///< Print 4-bit hex number. Outputs a single hex character.
void rprintfu08(unsigned char data);	///< Print 8-bit hex number. Outputs two hex characters.
void rprintfu16(unsigned short data);	///< Print 16-bit hex number. Outputs four hex characters.
void rprintfu32(unsigned long data);	///< Print 32-bit hex number. Outputs eight hex characters.

//! A flexible integer-number printing routine.
/// Print the number "n" in the given "base", using exactly "numDigits".
///	Print +/- if signed flag "isSigned" is TRUE.
///	The character specified in "padchar" will be used to pad extra characters.
///
///	Examples:
/// \code
/// uartPrintfNum(10, 6,  TRUE, ' ',   1234);  -->  " +1234"
///	uartPrintfNum(10, 6, FALSE, '0',   1234);  -->  "001234"
///	uartPrintfNum(16, 6, FALSE, '.', 0x5AA5);  -->  "..5AA5"
/// \endcode
void rprintfNum(char base, char numDigits, char isSigned, char padchar, long n);

#ifdef RPRINTF_FLOAT
	//! floating-point print routine
	void rprintfFloat(char numDigits, double x);
#endif

// NOTE: Below you'll see the function prototypes of rprintf1RamRom and 
// rprintf2RamRom.  rprintf1RamRom and rprintf2RamRom are both reduced versions
// of the regular C printf() command.  However, they are modified to be able
// to read their text/format strings from RAM or ROM in the Atmel microprocessors.
// Unless you really intend to, do not use the "RamRom" versions of the functions
// directly.  Instead use the #defined function versions:
//
// printfx("text/format",args)    ...to keep your text/format string stored in RAM
//		- or -
// printfxROM("text/format",args) ...to keep your text/format string stored in ROM
//
// where x is either 1 or 2 for the simple or more powerful version of printf()
//
// Since there is much more ROM than RAM available in the Atmel microprocessors,
// and nearly all text/format strings are constant (never change in the course
// of the program), you should try to use the ROM printf version exclusively.
// This will ensure you leave as much RAM as possible for program variables and
// data.

//! \fn int rprintf(const char *format, ...);
/// A reduced substitute for the usual C printf() function.
/// This function actually points to either rprintf1RamRom or rprintf2RamRom
/// depending on the user's selection.  Rprintf1 is a simple small fast print
/// routine while rprintf2 is larger and slower but more capable. To choose
/// the routine you would like to use, define either RPRINTF_SIMPLE or
/// RPRINTF_COMPLEX in global.h.

#ifdef RPRINTF_SIMPLE
	//! A simple printf routine.
	/// Called by rprintf() - does a simple printf (supports %d, %x, %c).
	/// Supports:
	/// - %d - decimal
	/// - %x - hex
	/// - %c - character
	int rprintf1RamRom(unsigned char stringInRom, const char *format, ...);
	// #defines for RAM or ROM operation
	#define rprintf1(format, args...)  		rprintf1RamRom(STRING_IN_ROM, PSTR(format), ## args)
	#define rprintf1RAM(format, args...)	rprintf1RamRom(STRING_IN_RAM, format, ## args)

	// *** Default rprintf(...) ***
	// this next line determines what the the basic rprintf() defaults to:
	#define rprintf(format, args...)  		rprintf1RamRom(STRING_IN_ROM, PSTR(format), ## args)
#endif

#ifdef RPRINTF_COMPLEX
	//! A more powerful printf routine.
	/// Called by rprintf() - does a more powerful printf (supports %d, %u, %o, %x, %c, %s).
	/// Supports:
	/// - %d - decimal
	/// - %u - unsigned decimal
	/// - %o - octal
	/// - %x - hex
	/// - %c - character
	/// - %s - strings
	/// - and the width,precision,padding modifiers
	/// \note This printf does not support floating point numbers.
	int rprintf2RamRom(unsigned char stringInRom, const char *sfmt, ...);
	// #defines for RAM or ROM operation
	#define rprintf2(format, args...)		rprintf2RamRom(STRING_IN_ROM, format, ## args)
	#define rprintf2RAM(format, args...)	rprintf2RamRom(STRING_IN_RAM, format, ## args)

	// *** Default rprintf(...) ***
	// this next line determines what the the basic rprintf() defaults to:
	#define rprintf(format, args...)  		rprintf2RamRom(STRING_IN_ROM, PSTR(format), ## args)
#endif

#endif
//@}
