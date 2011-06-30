/*! \file uart2.h \brief Dual UART driver with buffer support. */
//*****************************************************************************
//
// File Name	: 'uart2.h'
// Title		: Dual UART driver with buffer support
// Author		: Pascal Stang - Copyright (C) 2000-2002
// Created		: 11/20/2000
// Revised		: 07/04/2004
// Version		: 1.0
// Target MCU	: ATMEL AVR Series
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
///	\ingroup driver_avr
/// \defgroup uart2 UART Driver/Function Library for dual-UART processors (uart2.c)
/// \code #include "uart2.h" \endcode
/// \par Overview
///		This is a UART driver for AVR-series processors with two hardware
///		UARTs such as the mega161 and mega128. This library provides both
///		buffered and unbuffered transmit and receive functions for the AVR
///		processor UART. Buffered access means that the UART can transmit
///		and receive data in the "background", while your code continues
///		executing.  Also included are functions to initialize the UARTs,
///		set the baud rate, flush the buffers, and check buffer status.
///
/// \note	For full text output functionality, you may wish to use the rprintf
///		functions along with this driver.
///
/// \par About UART operations
///		Most Atmel AVR-series processors contain one or more hardware UARTs
///		(aka, serial ports).  UART serial ports can communicate with other 
///		serial ports of the same type, like those used on PCs.  In general,
///		UARTs are used to communicate with devices that are RS-232 compatible
///		(RS-232 is a certain kind of serial port).
///	\par
///		By far, the most common use for serial communications on AVR processors
///		is for sending information and data to a PC running a terminal program.
///		Here is an exmaple:
///	\code
/// uartInit();					// initialize UARTs (serial ports)
/// uartSetBaudRate(0, 9600);	// set UART0 speed to 9600 baud
/// uartSetBaudRate(1, 115200);	// set UART1 speed to 115200 baud
///
/// rprintfInit(uart0SendByte);	// configure rprintf to use UART0 for output
/// rprintf("Hello UART0\r\n");	// send "hello world" message via UART0
///
/// rprintfInit(uart1SendByte);	// configure rprintf to use UART1 for output
/// rprintf("Hello UART1\r\n");	// send "hello world" message via UART1
/// \endcode
///
/// \warning The CPU frequency (F_CPU) must be set correctly in \c global.h
///		for the UART library to calculate correct baud rates.  Furthermore,
///		certain CPU frequencies will not produce exact baud rates due to
///		integer frequency division round-off.  See your AVR processor's
///		 datasheet for full details.
//
//*****************************************************************************
//@{

#ifndef UART2_H
#define UART2_H

#include "global.h"
#include "buffer.h"

//! Default uart baud rate.
/// This is the default speed after a uartInit() command,
/// and can be changed by using uartSetBaudRate().
#define UART0_DEFAULT_BAUD_RATE		9600	///< default baud rate for UART0
#define UART1_DEFAULT_BAUD_RATE		9600	///< default baud rate for UART1

// buffer memory allocation defines
// buffer sizes
#ifndef UART0_TX_BUFFER_SIZE
#define UART0_TX_BUFFER_SIZE		0x0010	///< number of bytes for uart0 transmit buffer
#endif
#ifndef UART0_RX_BUFFER_SIZE
#define UART0_RX_BUFFER_SIZE		0x0080	///< number of bytes for uart0 receive buffer
#endif
#ifndef UART1_TX_BUFFER_SIZE
#define UART1_TX_BUFFER_SIZE		0x0010	///< number of bytes for uart1 transmit buffer
#endif
#ifndef UART1_RX_BUFFER_SIZE
#define UART1_RX_BUFFER_SIZE		0x0080	///< number of bytes for uart1 receive buffer
#endif

// define this key if you wish to use
// external RAM for the	UART buffers
//#define UART_BUFFER_EXTERNAL_RAM
#ifdef UART_BUFFER_EXTERNAL_RAM
	// absolute address of uart0 buffers
	#define UART0_TX_BUFFER_ADDR	0x1000
	#define UART0_RX_BUFFER_ADDR	0x1100
	// absolute address of uart1 buffers
	#define UART1_TX_BUFFER_ADDR	0x1200
	#define UART1_RX_BUFFER_ADDR	0x1300
#endif

//! Type of interrupt handler to use for uart interrupts.
/// Value may be SIGNAL or INTERRUPT.
/// \warning Do not change unless you know what you're doing.
#ifndef UART_INTERRUPT_HANDLER
#define UART_INTERRUPT_HANDLER	SIGNAL
#endif

// compatibility for the mega161
#ifndef RXCIE
	#define RXCIE	RXCIE0
	#define TXCIE	TXCIE0
	#define UDRIE	UDRIE0
	#define RXEN	RXEN0
	#define TXEN	TXEN0
	#define CHR9	CHR90
	#define RXB8	RXB80
	#define TXB8	TXB80
#endif
#ifndef UBRR0L
	#define UBRR0L	UBRR0
	#define UBRR1L	UBRR1
#endif

// functions

//! Initializes UARTs.
/// \note	After running this init function, the processor
/// I/O pins that used for uart communications (RXD, TXD)
/// are no long available for general purpose I/O.
void uartInit(void);

//! Initializes UART0 only.
void uart0Init(void);

//! Initializes UART1 only.
void uart1Init(void);

//! Initializes transmit and receive buffers.
/// Automatically called from uartInit()
void uart0InitBuffers(void);
void uart1InitBuffers(void);

//! Redirects received data to a user function.
///
void uartSetRxHandler(u08 nUart, void (*rx_func)(unsigned char c));

//! Sets the uart baud rate.
/// Argument should be in bits-per-second, like \c uartSetBaudRate(9600);
void uartSetBaudRate(u08 nUart, u32 baudrate);

//! Returns pointer to the receive buffer structure.
///
cBuffer* uartGetRxBuffer(u08 nUart);

//! Returns pointer to the transmit buffer structure.
///
cBuffer* uartGetTxBuffer(u08 nUart);

//! Sends a single byte over the uart.
///
void uartSendByte(u08 nUart, u08 data);

//! SendByte commands with the UART number hardcoded
/// Use these with printfInit() - example: \c printfInit(uart0SendByte);
void uart0SendByte(u08 data);
void uart1SendByte(u08 data);

//! Gets a single byte from the uart receive buffer.
/// Returns the byte, or -1 if no byte is available (getchar-style).
int uart0GetByte(void);
int uart1GetByte(void);

//! Gets a single byte from the uart receive buffer.
/// Function returns TRUE if data was available, FALSE if not.
/// Actual data is returned in variable pointed to by "data".
/// Example usage:
/// \code
/// char myReceivedByte;
/// uartReceiveByte(0, &myReceivedByte );
/// \endcode
u08 uartReceiveByte(u08 nUart, u08* data);

//! Returns TRUE/FALSE if receive buffer is empty/not-empty.
///
u08 uartReceiveBufferIsEmpty(u08 nUart);

//! Flushes (deletes) all data from receive buffer.
///
void uartFlushReceiveBuffer(u08 nUart);

//! Add byte to end of uart Tx buffer.
///
void uartAddToTxBuffer(u08 nUart, u08 data);

//! AddToTxBuffer commands with the UART number hardcoded
/// Use this with printfInit() - example: \c printfInit(uart0AddToTxBuffer);
void uart0AddToTxBuffer(u08 data);
void uart1AddToTxBuffer(u08 data);

//! Begins transmission of the transmit buffer under interrupt control.
///
void uartSendTxBuffer(u08 nUart);

//! sends a buffer of length nBytes via the uart using interrupt control.
///
u08 uartSendBuffer(u08 nUart, char *buffer, u16 nBytes);

//! interrupt service handlers
void uartTransmitService(u08 nUart);
void uartReceiveService(u08 nUart);

#endif

