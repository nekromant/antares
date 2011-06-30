/*! \file ip.h \brief IP (Internet Protocol) Library. */
//*****************************************************************************
//
// File Name	: 'ip.h'
// Title		: IP (Internet Protocol) Library
// Author		: Pascal Stang
// Created		: 8/30/2004
// Revised		: 7/3/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
///	\ingroup network
///	\defgroup ip IP (Internet Protocol) Library (ip.c)
///	\code #include "net/ip.h" \endcode
///	\par Description
///		The IP (Internet Protocol) library provide support for sending IP and
///		IP-related packets.  It's not clear if additional features are needed
///		or will be added, or even if this is the proper way to facilitate IP
///		packet operations.
//
//	This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//*****************************************************************************
//@{

#ifndef IP_H
#define IP_H

#include "net.h"
#include "arp.h"

#include <inttypes.h>

struct ipConfig				///< IP addressing/configuration structure
{
	uint32_t ip;			///< IP address
	uint32_t netmask;		///< netmask
	uint32_t gateway;		///< gateway IP address
};

#define IP_TIME_TO_LIVE		128		///< default Time-To-Live (TTL) value to use in IP headers

//! Set our IP address and routing information.
/// The myIp value will be used in the source field of IP packets.
/// Use this function to set and reset the system IP address.
void ipSetConfig(uint32_t myIp, uint32_t netmask, uint32_t gatewayIp);

//! Get our local IP address.
/// Returns current IP address value.
//uint32_t ipGetMyAddress(void);

//! Get our local IP configuration.
/// Returns pointer to current IP address/configuration.
struct ipConfig* ipGetConfig(void);

//! Print IP configuration
///
void ipPrintConfig(struct ipConfig* config);


//! Send an IP packet.
void ipSend(uint32_t dstIp, uint8_t protocol, uint16_t len, uint8_t* data);

//! Send a UDP/IP packet.
void udpSend(uint32_t dstIp, uint16_t dstPort, uint16_t len, uint8_t* data);

#endif
//@}

