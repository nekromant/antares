/*! \file net.h \brief Network support library. */
//*****************************************************************************
//
// File Name	: 'net.h'
// Title		: Network support library
// Author		: Pascal Stang
// Created		: 8/30/2004
// Revised		: 7/3/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
///	\ingroup network
///	\defgroup net Network support library (net.c)
///	\code #include "net/net.h" \endcode
///	\par Description
///		This is a general network support library including a multitude of 
///		structure definitions for various types of network packets, functions
///		and macros for switching byte order, and an RFC-compliant function
///		for calculating checksums.
//
//	This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//*****************************************************************************
//@{

#ifndef NET_H
#define NET_H

#include <inttypes.h>
#include "global.h"

// Representation of a 48-bit Ethernet address.
struct netEthAddr
{
	uint8_t addr[6];
} GNUC_PACKED;

// The Ethernet header
struct netEthHeader
{
	struct netEthAddr dest;
	struct netEthAddr src;
	uint16_t type;
} GNUC_PACKED;

#define ETH_HEADER_LEN	14

#define ETHTYPE_ARP 0x0806
#define ETHTYPE_IP  0x0800
#define ETHTYPE_IP6 0x86dd 

// The ARP header
struct netArpHeader
{
	uint16_t	hwtype;
	uint16_t	protocol;
	uint8_t		hwlen;
	uint8_t		protolen;
	uint16_t	opcode;
	struct		netEthAddr shwaddr;
	uint32_t	sipaddr;
	struct		netEthAddr dhwaddr;
	uint32_t	dipaddr; 
} GNUC_PACKED;

#define ARP_OPCODE_REQUEST	1
#define ARP_OPCODE_REPLY	2
#define ARP_HWTYPE_ETH		1

// The IP header
struct netIpHeader
{
	uint8_t		vhl;
	uint8_t		tos;
	uint16_t	len;
	uint16_t	ipid;
	uint16_t	ipoffset;
	uint8_t		ttl;
	uint8_t		proto;
	uint16_t	ipchksum;
	uint32_t	srcipaddr;
	uint32_t	destipaddr;
} GNUC_PACKED;
#define IP_HEADER_LEN	20

#define IP_PROTO_ICMP	1
#define IP_PROTO_TCP	6
#define IP_PROTO_UDP	17

// The ICMP header
struct netIcmpHeader
{
	uint8_t		type;
	uint8_t		icode;
	uint16_t	icmpchksum;
	uint16_t	id;
	uint16_t	seqno;
} GNUC_PACKED;
#define ICMP_HEADER_LEN	8

#define ICMP_TYPE_ECHOREPLY		0
#define ICMP_TYPE_ECHOREQUEST	8

// The UDP header
struct netUdpHeader
{
	uint16_t	srcport;
	uint16_t	destport;
	uint16_t	udplen;
	uint16_t	udpchksum;
} GNUC_PACKED;
#define UDP_HEADER_LEN	8

// The TCP header
struct netTcpHeader
{
	uint16_t	srcport;
	uint16_t	destport;
	uint32_t	seqno;
	uint32_t	ackno;
	uint8_t		tcpoffset;
	uint8_t		flags;
	uint16_t	wnd;
	uint16_t	tcpchksum;
	uint16_t	urgp;
//	uint8_t		optdata[4];
} GNUC_PACKED;
#define TCP_HEADER_LEN	20

#define TCP_FLAGS_FIN	0x01
#define TCP_FLAGS_SYN	0x02
#define TCP_FLAGS_RST	0x04
#define TCP_FLAGS_PSH	0x08
#define TCP_FLAGS_ACK	0x10
#define TCP_FLAGS_URG	0x20

// Ethernet/ARP header
struct netEthArpHeader
{
	struct netEthHeader eth;
	struct netArpHeader arp;
} GNUC_PACKED;

// Ethernet/IP header
struct netEthIpHeader
{
	struct netEthHeader eth;
	struct netIpHeader  ip;
} GNUC_PACKED;

// The IP header
typedef struct netIpHeader ip_hdr;

// The IP/TCP headers
typedef struct
{
	struct netIpHeader  ip;
	struct netTcpHeader tcp;
} tcpip_hdr;

// The IP/ICMP headers
typedef struct {
	struct netIpHeader  ip;
	struct netIcmpHeader icmp;
} icmpip_hdr;


// The UDP and IP headers
typedef struct {
	struct netIpHeader  ip;
	struct netUdpHeader udp;
} udpip_hdr;


//! Convert dot-notation IP address into 32-bit word.
/// Example: IPDOT(192l,168l,1l,1l)
#define IPDOT(a,b,c,d)	((a<<24)|(b<<16)|(c<<8)|(d))

//! Host-to-Network SHORT (16-bit) byte-order swap (macro).
#define HTONS(s)		((s<<8) | (s>>8))
//! Host-to-Network LONG (32-bit) byte-order swap (macro).
#define HTONL(l)		((l<<24) | ((l&0x00FF0000l)>>8) | ((l&0x0000FF00l)<<8) | (l>>24))

//! Host-to-Network SHORT (16-bit) byte-order swap (function).
uint16_t htons(uint16_t val);
//! Host-to-Network LONG (32-bit) byte-order swap (function).
uint32_t htonl(uint32_t val);

//! Calculate IP-style checksum from data.
uint16_t netChecksum(void *data, uint16_t len);

//! Print Ethernet address in XX:XX:XX:XX:XX:XX format.
void netPrintEthAddr(struct netEthAddr* ethaddr);
//! Print IP address in dot notation.
void netPrintIPAddr(uint32_t ipaddr);
//! Print Ethernet header information.
void netPrintEthHeader(struct netEthHeader* eth_hdr);
//! Print IP header information.
void netPrintIpHeader(struct netIpHeader* ipheader);
//! Print TCP header information.
void netPrintTcpHeader(struct netTcpHeader* tcpheader);

#endif
//@}

