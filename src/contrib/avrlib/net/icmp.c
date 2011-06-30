/*! \file icmp.c \brief ICMP Protocol Library. */
//*****************************************************************************
//
// File Name	: 'icmp.c'
// Title		: ICMP (Internet Control Message Protocol) Protocol Library
// Author		: Pascal Stang
// Created		: 9/10/2004
// Revised		: 7/3/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
//*****************************************************************************

#include "global.h"
#include "net.h"
#include "nic.h"
#include "arp.h"
#include "icmp.h"

#include "rprintf.h"
#include "debug.h"

//extern void nicSend(unsigned int len, unsigned char* packet);

// global variables


// functions
void icmpInit(void)
{
}

void icmpIpIn(icmpip_hdr* packet)
{
	// check ICMP type
	switch(packet->icmp.type)
	{
	case ICMP_TYPE_ECHOREQUEST:
		// echo request
		icmpEchoRequest(packet);
		break;
	default:
		break;
	}
}

void icmpEchoRequest(icmpip_hdr* packet)
{
	uint32_t tempIp;
	
	// change type to reply
	packet->icmp.type = ICMP_TYPE_ECHOREPLY;
	// recalculate checksum
	packet->icmp.icmpchksum = 0;
	packet->icmp.icmpchksum = netChecksum((u08*)&packet->icmp, htons(packet->ip.len)-IP_HEADER_LEN);
	// return to sender
	tempIp = packet->ip.destipaddr;
	packet->ip.destipaddr = packet->ip.srcipaddr;
	packet->ip.srcipaddr = tempIp;
	// add ethernet routing
	arpIpOut((struct netEthIpHeader*)(((u08*)packet)-ETH_HEADER_LEN), 0);
	
	// debugging
	#if NET_DEBUG >= 2
		icmpPrintHeader(packet);
		//debugPrintHexTable(htons(packet->ip.len), (u08*)packet);
	#endif
	
	// send it (packet->ip.len+ETH_HEADER_LEN
	nicSend(htons(packet->ip.len)+ETH_HEADER_LEN, (((u08*)packet)-ETH_HEADER_LEN));
}

#ifdef ICMP_DEBUG_PRINT
void icmpPrintHeader(icmpip_hdr* packet)
{
	rprintfProgStrM("ICMP Packet:\r\n");
	// print source IP address
	rprintfProgStrM("SrcIpAddr: ");	netPrintIPAddr(htonl(packet->ip.srcipaddr));	rprintfCRLF();
	// print dest IP address
	rprintfProgStrM("DstIpAddr: ");	netPrintIPAddr(htonl(packet->ip.destipaddr));	rprintfCRLF();
	// print type
	rprintfProgStrM("Type   : ");
	switch(packet->icmp.type)
	{
	case ICMP_TYPE_ECHOREQUEST:		rprintfProgStrM("ECHO REQUEST"); break;
	case ICMP_TYPE_ECHOREPLY:		rprintfProgStrM("ECHO REPLY"); break;
	default:						rprintfProgStrM("UNKNOWN"); break;
	}
	rprintfCRLF();
	// print code
	rprintfProgStrM("Code   : 0x");	rprintfu08(packet->icmp.icode);			rprintfCRLF();
}
#endif
