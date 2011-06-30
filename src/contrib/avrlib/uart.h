/*! \file uart.h \brief UART driver with buffer support. */
//*****************************************************************************
//
// File Name	: 'uart.h'
// Title		: UART driver with buffer support
// Author		: Pascal Stang - Copyright (C) 2000-2002
// Created		: 11/22/2000
// Revised		: 02/01/2004
// Version		: 1.3
// Target MCU	: ATMEL AVR Series
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
///	\ingroup driver_avr
/// \defgroup uart UART Driver/Function Library (uart.c)
/// \code #include "uart.h" \endcode
/// \par Overview
///		This library provides both buffered and unbuffered transmit and receive
///		functions for the AVR processor UART.  Buffered access means that the
///		UART can transmit and receive data in the "background", while your code
///		continues executing.  Also included are functions to initialize the
///		UART, set the baud rate, flush the buffers, and check buffer status.
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
/// uartInit();					// initialize UART (serial port)
/// uartSetBaudRate(9600);		// set UART speed to 9600 baud
/// rprintfInit(uartSendByte);  // configure rprintf to use UART for output
/// rprintf("Hello World\r\n");	// send "hello world" message via serial port
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

#ifndef UART_H
#define UART_H

#include "global.h"
#include "buffer.h"

//! Default uart baud rate.
/// This is the default speed after a uartInit() command,
/// and can be changed by using uartSetBaudRate().
#define UART_DEFAULT_BAUD_RATE	9600

// buffer memory allocation defines
// buffer sizes
#ifndef UART_TX_BUFFER_SIZE
//! Number of bytes for uart transmit buffer.
/// Do not change this value in uart.h, but rather override
/// it with the desired value defined in your project's global.h
#define UART_TX_BUFFER_SIZE		0x0040
#endif
#ifndef UART_RX_BUFFER_SIZE
//! Number of bytes for uart receive buffer.
/// Do not change this value in uart.h, but rather override
/// it with the desired value defined in your project's global.h
#define UART_RX_BUFFER_SIZE		0x0040
#endif

// define this key if you wish to use
// external RAM for the	UART buffers
//#define UART_BUFFER_EXTERNAL_RAM
#ifdef UART_BUFFER_EXTERNAL_RAM
	// absolute address of uart buffers
	#define UART_TX_BUFFER_ADDR	0x1000
	#define UART_RX_BUFFER_ADDR	0x1100
#endif

//! Type of interrupt handler to use for uart interrupts.
/// Value may be SIGNAL or INTERRUPT.
/// \warning Do not change unless you know what you're doing.
#ifndef UART_INTERRUPT_HANDLER
#define UART_INTERRUPT_HANDLER	SIGNAL
#endif

// compatibility with most newer processors
#ifdef UCSRB
	#define UCR					UCSRB
#endif
// compatibility with old Mega processors
#if defined(UBRR) && !defined(UBRRL)
	#define	UBRRL				UBRR
#endif
// compatibility with megaXX8 processors
#if	defined(__AVR_ATmega88__)	|| \
	defined(__AVR_ATmega168__)	|| \
	defined(__AVR_ATmega644__)
	#define UDR					UDR0
	#define UCR					UCSR0B
	#define RXCIE				RXCIE0
	#define TXCIE				TXCIE0
	#define RXC					RXC0
	#define TXC					TXC0
	#define RXEN				RXEN0
	#define TXEN				TXEN0
	#define UBRRL				UBRR0L
	#define UBRRH				UBRR0H
	#define SIG_UART_TRANS		SIG_USART_TRANS
	#define SIG_UART_RECV		SIG_USART_RECV
	#define SIG_UART_DATA		SIG_USART_DATA
#endif
// compatibility with mega169 processors
#if	defined(__AVR_ATmega169__)
	#define SIG_UART_TRANS		SIG_USART_TRANS
	#define SIG_UART_RECV		SIG_USART_RECV
	#define SIG_UART_DATA		SIG_USART_DATA
#endif
// compatibility with dual-uart processors
// (if you need to use both uarts, please use the uart2 library)
#if defined(__AVR_ATmega161__)
	#define UDR					UDR0
	#define UCR					UCSR0B
	#define UBRRL				UBRR0
	#define SIG_UART_TRANS		SIG_UART0_TRANS
	#define SIG_UART_RECV		SIG_UART0_RECV
	#define SIG_UART_DATA		SIG_UART0_DATA
#endif
#if defined(__AVR_ATmega128__)
#ifdef UART_USE_UART1
	#define UDR					UDR1
	#define UCR					UCSR1B
	#define UBRRL				UBRR1L
	#define UBRRH				UBRR1H
	#define SIG_UART_TRANS		SIG_UART1_TRANS
	#define SIG_UART_RECV		SIG_UART1_RECV
	#define SIG_UART_DATA		SIG_UART1_DATA
#else
	#define UDR					UDR0
	#define UCR					UCSR0B
	#define UBRRL				UBRR0L
	#define UBRRH				UBRR0H
	#define SIG_UART_TRANS		SIG_UART0_TRANS
	#define SIG_UART_RECV		SIG_UART0_RECV
	#define SIG_UART_DATA		SIG_UART0_DATA
#endif
#endif

// functions

//! Initializes uart.
/// \note	After running this init function, the processor
/// I/O pins that used for uart communications (RXD, TXD)
/// are no long available for general purpose I/O.
void uartInit(void);

//! Initializes transmit and receive buffers.
/// Automatically called from uartInit()
void uartInitBuffers(void);

//! Redirects received data to a user function.
///
void uartSetRxHandler(void (*rx_func)(unsigned char c));

//! Sets the uart baud rate.
/// Argument should be in bits-per-second, like \c uartSetBaudRate(9600);
void uartSetBaudRate(u32 baudrate);

//! Returns pointer to the receive buffer structure.
///
cBuffer* uartGetRxBuffer(void);

//! Returns pointer to the transmit buffer structure.
///
cBuffer* uartGetTxBuffer(void);

//! Sends a single byte over the uart.
/// \note This function waits for the uart to be ready,
/// therefore, consecutive calls to uartSendByte() will
/// go only as fast as the data can be sent over the
/// serial port.
void uartSendByte(u08 data);

//! Gets a single byte from the uart receive buffer.
/// Returns the byte, or -1 if no byte is available (getchar-style).
int uartGetByte(void);

//! Gets a single byte from the uart receive buffer.
/// Function returns TRUE if data was available, FALSE if not.
/// Actual data is returned in variable pointed to by "data".
/// Example usage:
/// \code
/// char myReceivedByte;
/// uartReceiveByte( &myReceivedByte );
/// \endcode
u08 uartReceiveByte(u08* data);

//! Returns TRUE/FALSE if receive buffer is empty/not-empty.
///
u08 uartReceiveBufferIsEmpty(void);

//! Flushes (deletes) all data from receive buffer.
///
void uartFlushReceiveBuffer(void);

//! Add byte to end of uart Tx buffer.
///	Returns TRUE if successful, FALSE if failed (no room left in buffer).
u08 uartAddToTxBuffer(u08 data);

//! Begins transmission of the transmit buffer under interrupt control.
///
void uartSendTxBuffer(void);

//! Sends a block of data via the uart using interrupt control.
/// \param buffer	pointer to data to be sent
///	\param nBytes	length of data (number of bytes to sent)
u08  uartSendBuffer(char *buffer, u16 nBytes);

#endif
//@}


