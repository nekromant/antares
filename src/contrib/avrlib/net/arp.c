/*! \file arp.c \brief ARP Protocol Library. */
//*****************************************************************************
//
// File Name	: 'arp.c'
// Title		: ARP Protocol Library
// Author		: Pascal Stang
// Created		: 9/7/2004
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

#include "rprintf.h"

// global variables

/// Single ARP table entry/record
struct ArpEntry
{
	uint32_t ipaddr;			///< remote-note IP address
	struct netEthAddr ethaddr;	///< remote-node ethernet (hardware/mac) address
	uint8_t time;				///< time to live (in ARP table); this is decremented by arpTimer()
};

struct ArpEntry ArpMyAddr;		///< my local interface information (IP and MAC address)
struct ArpEntry ArpTable[ARP_TABLE_SIZE];	///< ARP table of matched IP<->MAC associations


void arpInit(void)
{
	u08 i;
	// initialize all ArpTable elements to unused
	for(i=0; i<ARP_TABLE_SIZE; i++)
	{
		ArpTable[i].ipaddr = 0;
		ArpTable[i].time = 0;
	}
}

void arpSetAddress(struct netEthAddr* myeth, uint32_t myip)
{
	// set local address record
	ArpMyAddr.ethaddr = *myeth;
	ArpMyAddr.ipaddr = myip;
}

void arpArpIn(unsigned int len, struct netEthArpHeader* packet)
{
	#ifdef ARP_DEBUG
	rprintfProgStrM("Received ARP Request\r\n");
	arpPrintHeader( &packet->arp );
	#endif

	// for now, we just reply to requests
	// need to add ARP cache
	if(	(packet->arp.dipaddr == HTONL(ArpMyAddr.ipaddr)) &&
		(packet->arp.opcode == htons(ARP_OPCODE_REQUEST)) )
	{
		// in ARP header
		// copy sender's address info to dest. fields
		packet->arp.dhwaddr = packet->arp.shwaddr;
		packet->arp.dipaddr = packet->arp.sipaddr;
		// fill in our information
		packet->arp.shwaddr = ArpMyAddr.ethaddr;
		packet->arp.sipaddr = HTONL(ArpMyAddr.ipaddr);
		// change op to reply
		packet->arp.opcode = htons(ARP_OPCODE_REPLY);
		
		// in ethernet header
		packet->eth.dest = packet->eth.src;
		packet->eth.src  = ArpMyAddr.ethaddr;

		#ifdef ARP_DEBUG
		rprintfProgStrM("Sending ARP Reply\r\n");
		arpPrintHeader( &packet->arp );
		#endif

		// send reply!
		nicSend(len, (unsigned char*)packet);
	}
}

void arpIpIn(struct netEthIpHeader* packet)
{
	int8_t index;

	// check if sender is already present in arp table
	index = arpMatchIp(HTONL(packet->ip.srcipaddr));
	if(index != -1)
	{
		// sender's IP address found, update ARP entry
		ArpTable[index].ethaddr = packet->eth.src;
		// and we're done
		return;
	}

	// sender was not present in table,
	// must add in empty/expired slot
	for(index=0; index<ARP_TABLE_SIZE; index++)
	{
		if(!ArpTable[index].time)
		{
			// write entry
			ArpTable[index].ethaddr = packet->eth.src;
			ArpTable[index].ipaddr = HTONL(packet->ip.srcipaddr);
			ArpTable[index].time = ARP_CACHE_TIME_TO_LIVE;
			// and we're done
			return;
		}
	}

	// no space in table, we give up
}

void arpIpOut(struct netEthIpHeader* packet, uint32_t phyDstIp)
{
	int index;
	// check if destination is already present in arp table
	// use the physical dstIp if it's provided, otherwise the dstIp in packet
	if(phyDstIp)
		index = arpMatchIp(phyDstIp);
	else
		index = arpMatchIp(HTONL(packet->ip.destipaddr));
	// fill in ethernet info
	if(index != -1)
	{
		// ARP entry present, fill eth address(es)
		packet->eth.src  = ArpMyAddr.ethaddr;
		packet->eth.dest = ArpTable[index].ethaddr;
		packet->eth.type = HTONS(ETHTYPE_IP);
	}
	else
	{
		// not in table, must send ARP request
		packet->eth.src = ArpMyAddr.ethaddr;
		// MUST CHANGE, but for now, send this one broadcast
		packet->eth.dest.addr[0] = 0xFF;
		packet->eth.dest.addr[1] = 0xFF;
		packet->eth.dest.addr[2] = 0xFF;
		packet->eth.dest.addr[3] = 0xFF;
		packet->eth.dest.addr[4] = 0xFF;
		packet->eth.dest.addr[5] = 0xFF;
		packet->eth.type = HTONS(ETHTYPE_IP);
	}
}

void arpTimer(void)
{
	int index;
	// this function meant to be called on a regular time interval

	// decrement time-to-live for all entries
	for(index=0; index<ARP_TABLE_SIZE; index++)
	{
		if(ArpTable[index].time)
			ArpTable[index].time--;
	}
}

int arpMatchIp(uint32_t ipaddr)
{
	uint8_t i;

	// check if IP address is present in arp table
	for(i=0; i<ARP_TABLE_SIZE; i++)
	{
		if(ArpTable[i].ipaddr == ipaddr)
		{
			// IP address found
			return i;
		}
	}

	// no match
	return -1;
}

#ifdef ARP_DEBUG_PRINT
void arpPrintHeader(struct netArpHeader* packet)
{
	rprintfProgStrM("ARP Packet:\r\n");
	//debugPrintHexTable(60, (unsigned char*)&packet);
	// print operation type
	rprintfProgStrM("Operation   : ");
	if(packet->opcode == htons(ARP_OPCODE_REQUEST))
		rprintfProgStrM("REQUEST");
	else if(packet->opcode == htons(ARP_OPCODE_REPLY))
		rprintfProgStrM("REPLY");
	else
		rprintfProgStrM("UNKNOWN");
	rprintfCRLF();
	// print source hardware address
	rprintfProgStrM("SrcHwAddr   : ");	netPrintEthAddr(&packet->shwaddr);	rprintfCRLF();
	// print source protocol address
	rprintfProgStrM("SrcProtoAddr: ");	netPrintIPAddr(HTONL(packet->sipaddr));	rprintfCRLF();
	// print target hardware address
	rprintfProgStrM("DstHwAddr   : ");	netPrintEthAddr(&packet->dhwaddr);	rprintfCRLF();
	// print target protocol address
	rprintfProgStrM("DstProtoAddr: ");	netPrintIPAddr(HTONL(packet->dipaddr));	rprintfCRLF();
}


void arpPrintTable(void)
{
	uint8_t i;

	// print ARP table
	rprintfProgStrM("Time    Eth Address    IP Address\r\n");
	rprintfProgStrM("---------------------------------------\r\n");
	for(i=0; i<ARP_TABLE_SIZE; i++)
	{
		rprintfu08(ArpTable[i].time);
		rprintfProgStrM("   ");
		netPrintEthAddr(&ArpTable[i].ethaddr);
		rprintfProgStrM("  ");
		netPrintIPAddr(ArpTable[i].ipaddr);
		rprintfCRLF();
	}
}
#endif
