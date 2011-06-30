//*****************************************************************************
// File Name	: netstacktest.c
// 
// Title		: example usage of network library functions
// Revision		: 1.0
// Notes		:	
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
// 
// Revision History:
// When			Who			Description of change
// -----------	-----------	-----------------------
// 10-Aug-2005	pstang		Created the program
//*****************************************************************************

#include <avr/io.h>
#include <string.h>
#include <stdlib.h>

// include avrlib basics
#include "global.h"
#include "timer.h"
#include "uart.h"
#include "rprintf.h"
#include "vt100.h"
#include "debug.h"

// include network support
#include "net/net.h"
#include "net/nic.h"
#include "net/arp.h"
#include "net/icmp.h"
#include "net/ip.h"
#include "net/netstack.h"

// network defines
#define IPADDRESS			IPDOT(192l,168l,1l,12l)
#define NETMASK				IPDOT(255l,255l,255l,0l)
#define GATEWAY				IPDOT(192l,168l,1l,1l)

#define LOOPBACK_PORT		7		// UDP packets sent to this port will be returned to sender
#define CONTROL_PORT		4950	// UDP packets sent to this port will be used for control
#define SERIAL_PORT			4951	// UDP packets sent to this port will be printed via serial

// timer defines
#define TIMER_PRESCALE		1024
#define TIMER_INTERVAL		(F_CPU/TIMER_PRESCALE/100)	// 100ms interval

// global variables
static volatile unsigned long UptimeMs;

// functions
void processCommand(u16 len, u08* data);
void serviceLocal(void);
void systickHandler(void);

// prototypes
int main(void)
{
	struct netEthAddr myEthAddress;

	timerInit();
	uartInit();
	uartSetBaudRate(115200);
	rprintfInit(uartSendByte);
	timerPause(100);
	vt100ClearScreen();
	rprintf("\r\nNetwork Stack Example\r\n");

	// initialize systick timer
	rprintf("Initializing Periodic Timer\r\n");
	timer2SetPrescaler(TIMERRTC_CLK_DIV1024);
	timerAttach(TIMER2OVERFLOW_INT, systickHandler);

	// init network stack
	rprintf("Initializing Network Stack\r\n");
	netstackInit(IPADDRESS, NETMASK, GATEWAY);

	nicGetMacAddress(&myEthAddress.addr[0]);
	rprintfProgStrM("Eth Addr is: "); netPrintEthAddr(&myEthAddress);		rprintfCRLF();
	rprintfProgStrM("IP  Addr is: "); netPrintIPAddr(ipGetConfig()->ip);	rprintfCRLF();

	rprintf("Network Stack is up!\r\n");
	rprintf("Starting packet receive loop\r\n");
	
	while(1)
	{
		// service local stuff
		serviceLocal();
		// service the network
		netstackService();
	}

	return 0;
}


void netstackUDPIPProcess(unsigned int len, udpip_hdr* packet)
{
	u16 payloadlen=0;
	u08* payloaddata=0;
	u16 i;

	// get UDP payload length
	payloadlen = htons(packet->udp.udplen);
	payloadlen -= 8; // subtract header
	// get UDP payload data
	payloaddata = &((unsigned char*)packet)[IP_HEADER_LEN+UDP_HEADER_LEN];
	
	rprintf("UDP packet, len: %d\r\n", len);
//	debugPrintHexTable(len, (unsigned char*)packet);

	if(packet->udp.destport == HTONS(CONTROL_PORT))
	{
		// command packet
		processCommand(payloadlen, payloaddata);
	}
	else if(packet->udp.destport == HTONS(SERIAL_PORT))
	{
		// serial output
		for(i=0; i<payloadlen; i++)
			uartSendByte(payloaddata[i]);
	}
	else if(packet->udp.destport == HTONS(LOOPBACK_PORT))
	{
		// loopback - return packet to sender
		udpSend(htonl(packet->ip.srcipaddr), LOOPBACK_PORT, payloadlen, payloaddata);
	}
}


void netstackTCPIPProcess(unsigned int len, tcpip_hdr* packet)
{
	rprintf("Received TCP/IP Packet: len=%d\r\n", len);
}


void processCommand(u16 len, u08* data)
{
	rprintf("Received UDP command: CMD=0x%x  ARG0=0x%x\r\n", data[0], data[1]);

	// do something based on command
	switch(data[0])
	{
	case 'C':	// set PORTC
		PORTC = data[1];
		break;	// set DDRC
	case 'c':
		PORTC = data[1];
		break;
	default:
		rprintf("Unknown UDP command\r\n");
		break;
	}
}

void serviceLocal(void)
{
	int c;
	unsigned char buffer[100];

	if( (c = uartGetByte()) != -1)
	{
		// echo command to terminal
		rprintfChar(c);
		// process command
		switch(c)
		{
		case 'i':
			rprintfProgStrM("\r\nInitializing Ethernet Controller\r\n");
			nicInit();
			break;
		case 'd':
			rprintfProgStrM("\r\nEthernet Controller State\r\n");
			nicRegDump();
			break;
		case 't':
			rprintfProgStrM("\r\nCurrent Uptime: ");
			rprintfNum(10, 9, FALSE, ' ', UptimeMs);
			rprintfProgStrM("ms\r\n");
			break;
		case 'c':
			rprintfProgStrM("\r\nCrashing System....\r\n");
			while(1);
			break;
		case 'u':
			rprintfProgStrM("\r\nSending UDP packet\r\n");
			strcpy(&buffer[ETH_HEADER_LEN+IP_HEADER_LEN+UDP_HEADER_LEN], "hello");
			udpSend((ipGetConfig()->ip|~ipGetConfig()->netmask), CONTROL_PORT, 6, &buffer[ETH_HEADER_LEN+IP_HEADER_LEN+UDP_HEADER_LEN]);
			break;
		case '?':
			rprintfProgStrM("\r\nCommands:\r\n");
			rprintfProgStrM("(i) Initialize Ethernet Controller\r\n");
			rprintfProgStrM("(d) Dump Ethernet Controller State\r\n");
			rprintfProgStrM("(u) Send Broadcast UDP frame\r\n");
			rprintfProgStrM("(t) Print current uptime\r\n");
			rprintfProgStrM("(?) Help\r\n");
			break;
		case '\r':
		default:
			break;
		}
		// print new prompt
		rprintfProgStrM("\r\ncmd>");
	}
}

void systickHandler(void)
{
	// set timer for 10ms interval
	TCNT2 = (unsigned char)-TIMER_INTERVAL;
	// count up on uptime
	UptimeMs += 10;
}
