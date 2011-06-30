/*! \file netstack.h \brief Network Stack. */
//*****************************************************************************
//
// File Name	: 'netstack.h'
// Title		: Network Stack
// Author		: Pascal Stang
// Created		: 6/28/2005
// Revised		: 9/20/2005
// Version		: 0.3
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
///	\ingroup network
///	\defgroup netstack Network Stack (netstack.c)
///	\code #include "net/netstack.h" \endcode
///	\par Description
///		This library co-ordinates the various pieces of a typical IP network
///		stack into one unit.  Included are handling for ARP, ICMP, and IP
///		packets.  UDP and TCP packets are processed and passed to the user.
///
///		This is an example of how to use the various network libraries, and
///		is meant to be useful out-of-the-box for most users.  However, some
///		users may find it restrictive and write their own handlers instead.
///		This stack implementation is by no means the only way to use the various
///		network libraries.
///
///	\note This is NOT a full-blown TCP/IP stack.  It merely handles lower
///		level stack functions so that UDP and TCP packets can be sent
///		and received easily.  End-to-end TCP functionality may be added
///		in a future version.  Until then, I can recommend using other embedded
///		TCP/IP stacks like Adam Dunkel's uIP.
//
//	This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//*****************************************************************************
//@{

#ifndef NETSTACK_H
#define NETSTACK_H

#include "net.h"
#include "arp.h"
#include "icmp.h"
#include "ip.h"
#include "nic.h"

//#define NETSTACK_DEBUG

/// NET_BUFFERSIZE is the common receive/process/transmit buffer.
/// - You may override the default NET_BUFFERSIZE by defining an alternate value in global.h.
/// - Network packets larger than NET_BUFFERSIZE will not be accepted.
#ifndef NETSTACK_BUFFERSIZE
#define NETSTACK_BUFFERSIZE		(576+ETH_HEADER_LEN)
#endif

/// netstackInit prepares the network interface for use and should be called
/// once at the beginning of the user program.
/// \note Use ipSetAddress() to change network parameters in mid-run.
void netstackInit(uint32_t ipaddress, uint32_t netmask, uint32_t gatewayip);

/// netstackGetBuffer returns a pointer to the common receive/process/transmit buffer.
u08* netstackGetBuffer(void);

/// netstackService should be called in the main loop of the user program.
/// The function will process one received network packet per call.
/// The return value is the length of the packet processed, or zero if no
/// packet was processed.
int netstackService(void);

/// netstackIPProcess handles distribution of IP received packets.
///
void netstackIPProcess(unsigned int len, ip_hdr* packet);

/// This weakly-defined function is the default handler for incoming UDP/IP packets.
/// Users should define this same function in user code (same name and arguments) to
/// override this default handler and get access to the received packets.
void netstackUDPIPProcess(unsigned int len, udpip_hdr* packet) __attribute__ ((weak));

/// This weakly-defined function is the default handler for incoming TCP/IP packets.
/// Users should define this same function in user code (same name and arguments) to
/// override this default handler and get access to the received packets.
void netstackTCPIPProcess(unsigned int len, tcpip_hdr* packet) __attribute__ ((weak));

#endif
//@}
