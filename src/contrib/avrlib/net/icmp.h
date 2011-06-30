/*! \file icmp.h \brief ICMP Protocol Library. */
//*****************************************************************************
//
// File Name	: 'icmp.h'
// Title		: ICMP (Internet Control Message Protocol) Protocol Library
// Author		: Pascal Stang
// Created		: 9/10/2004
// Revised		: 7/3/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
///	\ingroup network
///	\defgroup icmp ICMP Protocol Library (icmp.c)
///	\code #include "net/icmp.h" \endcode
///	\par Description
///		ICMP (Internet Control Message Protocol) has many functions on the
///		internet, including the handling of ECHO (ping) requests, relaying
///		network route status, passing connection status messages, etc.
///
///		This library currently handles only ICMP ECHO requests (ping), but
///		may be expanded to include other useful ICMP operations as needed.
//
//	This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//*****************************************************************************
//@{

#ifndef ICMP_H
#define ICMP_H

#include "global.h"
#include "net.h"

//#define ICMP_DEBUG_PRINT

//! Initialize ICMP protocol library.
void icmpInit(void);

//! Incoming IP packets of protocol ICMP should be passed to this function.
void icmpIpIn(icmpip_hdr* packet);

//! Forms and sends a reply in response to an ICMP ECHO request.
void icmpEchoRequest(icmpip_hdr* packet);

//! Print ICMP packet information.
void icmpPrintHeader(icmpip_hdr* packet);

#endif
//@}
