/*! \file ip.c \brief IP (Internet Protocol) Library. */
//*****************************************************************************
//
// File Name	: 'ip.c'
// Title		: IP (Internet Protocol) Library
// Author		: Pascal Stang
// Created		: 8/30/2004
// Revised		: 7/3/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
//*****************************************************************************


#include <inttypes.h>
#include "global.h"
#include "rprintf.h"

#include "net.h"
#include "nic.h"
#include "arp.h"
#include "ip.h"

struct ipConfig IpMyConfig;	///< Local IP address/config structure


void ipSetConfig(uint32_t myIp, uint32_t netmask, uint32_t gatewayIp)
{
	struct netEthAddr ethaddr;

	// set local addressing
	IpMyConfig.ip = myIp;
	IpMyConfig.netmask = netmask;
	IpMyConfig.gateway = gatewayIp;

	// set ARP association
	nicGetMacAddress(ethaddr.addr);
	arpSetAddress(&ethaddr, myIp);
}

struct ipConfig* ipGetConfig(void)
{
	return &IpMyConfig;
}

// deprecated
/*
uint32_t ipGetMyAddress(void)
{
	return IpMyConfig.ip;
}
*/

void ipSend(uint32_t dstIp, uint8_t protocol, uint16_t len, uint8_t* data)
{
	// make pointer to ethernet/IP header
	struct netEthIpHeader* ethIpHeader;

	// move data pointer to make room for headers
	data -= ETH_HEADER_LEN+IP_HEADER_LEN;
	ethIpHeader = (struct netEthIpHeader*)data;

//	debugPrintHexTable(len+ETH_HEADER_LEN+IP_HEADER_LEN, data);

	// adjust length to add IP header
	len += IP_HEADER_LEN;

	// fill IP header
	ethIpHeader->ip.destipaddr = HTONL(dstIp);
	ethIpHeader->ip.srcipaddr = HTONL(IpMyConfig.ip);
	ethIpHeader->ip.proto = protocol;
	ethIpHeader->ip.len = htons(len);
	ethIpHeader->ip.vhl = 0x45;
	ethIpHeader->ip.tos = 0;
	ethIpHeader->ip.ipid = 0;
	ethIpHeader->ip.ipoffset = 0;
	ethIpHeader->ip.ttl = IP_TIME_TO_LIVE;
	ethIpHeader->ip.ipchksum = 0;

	// calculate and apply IP checksum
	// DO THIS ONLY AFTER ALL CHANGES HAVE BEEN MADE TO IP HEADER
	ethIpHeader->ip.ipchksum = netChecksum(&ethIpHeader->ip, IP_HEADER_LEN);

	// add ethernet routing
	// check if we need to send to gateway
	if( (dstIp & IpMyConfig.netmask) == (IpMyConfig.ip & IpMyConfig.netmask) )
	{
		arpIpOut(ethIpHeader,0);					// local send
		//rprintf("Sending IP packet on local net\r\n");
	}
	else
	{
		arpIpOut(ethIpHeader,IpMyConfig.gateway);	// gateway send
		//rprintf("Sending IP packet to gateway\r\n");
	}

	// adjust length to add ethernet header
	len += ETH_HEADER_LEN;

	// debug
	//debugPrintHexTable(ETH_HEADER_LEN, &data[0]);
	//debugPrintHexTable(len-ETH_HEADER_LEN, &data[ETH_HEADER_LEN]);

	// send it
	nicSend(len, data);
}

void udpSend(uint32_t dstIp, uint16_t dstPort, uint16_t len, uint8_t* data)
{
	// make pointer to UDP header
	struct netUdpHeader* udpHeader;
	// move data pointer to make room for UDP header
	data -= UDP_HEADER_LEN;
	udpHeader = (struct netUdpHeader*)data;
	// adjust length to add UDP header
	len += UDP_HEADER_LEN;
	// fill UDP header
	udpHeader->destport = HTONS(dstPort);
	udpHeader->srcport  = HTONS(dstPort);
	udpHeader->udplen = htons(len);
	udpHeader->udpchksum = 0;

//	debugPrintHexTable(UDP_HEADER_LEN, (uint8_t*)udpPacket);

	ipSend(dstIp, IP_PROTO_UDP, len, (uint8_t*)udpHeader);
}

void ipPrintConfig(struct ipConfig* config)
{
	rprintfProgStrM("IP Addr : "); netPrintIPAddr(config->ip);		rprintfCRLF();
	rprintfProgStrM("Netmask : "); netPrintIPAddr(config->netmask);	rprintfCRLF();
	rprintfProgStrM("Gateway : "); netPrintIPAddr(config->gateway);	rprintfCRLF();
}
