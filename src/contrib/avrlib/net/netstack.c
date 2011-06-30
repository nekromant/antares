/*! \file netstack.c \brief Network Stack. */
//*****************************************************************************
//
// File Name	: 'netstack.c'
// Title		: Network Stack
// Author		: Pascal Stang
// Created		: 6/28/2005
// Revised		: 9/20/2005
// Version		: 0.3
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
//*****************************************************************************

#include "rprintf.h"
#include "debug.h"

#include "netstack.h"

unsigned char NetBuffer[NETSTACK_BUFFERSIZE];

void netstackInit(uint32_t ipaddress, uint32_t netmask, uint32_t gatewayip)
{
	// init network device driver
	#ifdef NETSTACK_DEBUG
	rprintf("Initializing Network Device\r\n");
	#endif
	nicInit();
	// init ARP
	#ifdef NETSTACK_DEBUG
	rprintf("Initializing ARP cache\r\n");
	#endif
	arpInit();
	// init addressing
	#ifdef NETSTACK_DEBUG
	rprintf("Initializing Addressing\r\n");
	#endif
	ipSetConfig(ipaddress, netmask, gatewayip);
}

u08* netstackGetBuffer(void)
{
	return NetBuffer;
}

int netstackService(void)
{
	int len;
	struct netEthHeader* ethPacket;

	// look for a packet
	len = nicPoll(NETSTACK_BUFFERSIZE, NetBuffer);

	if(len)
	{
		ethPacket = (struct netEthHeader*)&NetBuffer[0];

		#if NET_DEBUG >= 5
		rprintf("Received packet len: %d, type:", len);
		rprintfu16(htons(ethPacket->type));
		rprintfCRLF();
		rprintf("Packet Contents\r\n");
		debugPrintHexTable(len, NetBuffer);
		#endif
		
		if(ethPacket->type == htons(ETHTYPE_IP))
		{
			// process an IP packet
			#ifdef NETSTACK_DEBUG
			rprintfProgStrM("NET Rx: IP packet\r\n");
			#endif
			// add the source to the ARP cache
			// also correctly set the ethernet packet length before processing?
			arpIpIn((struct netEthIpHeader*)&NetBuffer[0]);
			//arpPrintTable();
			
			netstackIPProcess( len-ETH_HEADER_LEN, (ip_hdr*)&NetBuffer[ETH_HEADER_LEN] );
		}
		else if(ethPacket->type == htons(ETHTYPE_ARP))
		{
			// process an ARP packet
			#ifdef NETSTACK_DEBUG
			rprintfProgStrM("NET Rx: ARP packet\r\n");
			#endif
			arpArpIn(len, ((struct netEthArpHeader*)&NetBuffer[0]) );
		}
	}
	return len;
}

void netstackIPProcess(unsigned int len, ip_hdr* packet)
{
	// check IP addressing, stop processing if not for me and not a broadcast
	if( (htonl(packet->destipaddr) != ipGetConfig()->ip) &&
		(htonl(packet->destipaddr) != (ipGetConfig()->ip|ipGetConfig()->netmask)) &&
		(htonl(packet->destipaddr) != 0xFFFFFFFF) ) 
		return;

	// handle ICMP packet
	if( packet->proto == IP_PROTO_ICMP )
	{
		#ifdef NETSTACK_DEBUG
		rprintfProgStrM("NET Rx: ICMP/IP packet\r\n");
		//icmpPrintHeader((icmpip_hdr*)packet);
		#endif
		icmpIpIn((icmpip_hdr*)packet);
	}
	else if( packet->proto == IP_PROTO_UDP )
	{
		#ifdef NETSTACK_DEBUG
		rprintfProgStrM("NET Rx: UDP/IP packet\r\n");
		//debugPrintHexTable(NetBufferLen-14, &NetBuffer[14]);
		#endif
		netstackUDPIPProcess(len, ((udpip_hdr*)packet) );
	}
	else if( packet->proto == IP_PROTO_TCP )
	{
		#ifdef NETSTACK_DEBUG
		rprintfProgStrM("NET Rx: TCP/IP packet\r\n");
		#endif
		netstackTCPIPProcess(len, ((tcpip_hdr*)packet) );
	}
	else
	{
		#ifdef NETSTACK_DEBUG
		rprintfProgStrM("NET Rx: IP packet\r\n");
		#endif
	}
}

void netstackUDPIPProcess(unsigned int len, udpip_hdr* packet)
{
	#ifdef NETSTACK_DEBUG
	rprintf("NetStack UDP/IP Rx Dummy Handler\r\n");
	#endif
}

void netstackTCPIPProcess(unsigned int len, tcpip_hdr* packet)
{
	#ifdef NETSTACK_DEBUG
	rprintf("NetStack TCP/IP Rx Dummy Handler\r\n");
	#endif
}
