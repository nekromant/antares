/*! \file prism2.h \brief Prism2 802.11b Wireless-LAN Interface Driver. */
//*****************************************************************************
//
// File Name	: 'prism2.h'
// Title		: Prism2 802.11b Wireless-LAN Interface Driver
// Author		: Pascal Stang
// Created		: 12/27/2004
// Revised		: 1/7/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
///	\ingroup network
///	\defgroup prism2 PrismII 802.11b WLAN Interface Driver (prism2.c)
///	\code #include "net/prism2.h" \endcode
///	\par Overview
///		This driver provides initialization and transmit/receive
///	functions for the Prism2 802.11b Wireless-LAN Controller.
///
/// \note This driver works but is still in development.
//
//*****************************************************************************
//@{

#ifndef PRISM2_H
#define PRISM2_H

#include "global.h"

#define nop()	asm volatile ("nop")

// PRISM2 I/O register defines
#define PRISM2_REG_CMD			0x00
#define PRISM2_REG_PARAM0		0x02
#define PRISM2_REG_PARAM1		0x04
#define PRISM2_REG_PARAM2		0x06
#define PRISM2_REG_STATUS		0x08
#define PRISM2_REG_RESP0		0x0A
#define PRISM2_REG_RESP1		0x0C
#define PRISM2_REG_RESP2		0x0E

#define PRISM2_REG_INFOFID		0x10
#define PRISM2_REG_RXFID		0x20
#define PRISM2_REG_ALLOCFID		0x22
#define PRISM2_REG_TXFID		0x24

#define PRISM2_REG_BAP0SEL		0x18
#define PRISM2_REG_BAP0OFFSET	0x1C
#define PRISM2_REG_BAP0DATA		0x36

#define PRISM2_REG_BAP1SEL		0x1A
#define PRISM2_REG_BAP1OFFSET	0x1E
#define PRISM2_REG_BAP1DATA		0x38

#define PRISM2_REG_EVSTAT		0x30
#define PRISM2_REG_INTEN		0x32
#define PRISM2_REG_EVACK		0x34

#define PRISM2_REG_SWSUP0       0x28
#define PRISM2_REG_SWSUP1       0x2A
#define PRISM2_REG_SWSUP2		0x2C

#define PRISM2_REG_AUXPAGE		0x3A
#define PRISM2_REG_AUXOFFSET	0x3C
#define PRISM2_REG_AUXDATA		0x3E


// PRISM2 commands defines
#define PRISM2_CMD_INIT			0x0000
#define PRISM2_CMD_ENABLE_MAC0	0x0001
#define PRISM2_CMD_DISABLE_MAC0	0x0002
#define PRISM2_CMD_DIAG			0x0003
#define PRISM2_CMD_ALLOC		0x000A
#define PRISM2_CMD_TX			0x000B
#define PRISM2_CMD_TX_RECL		0x010B
#define PRISM2_CMD_NOTIFY		0x0010
#define PRISM2_CMD_INQUIRE		0x0011
#define PRISM2_CMD_ACCESS_RD	0x0021
#define PRISM2_CMD_ACCESS_WR	0x0121
#define PRISM2_CMD_BUSY			0x8000
#define PRISM2_CMD_NORESP		0xFFFF

// PRISM2 command result codes
#define PRISM2_RESULT_SUCCESS	0x00
#define PRISM2_RESULT_CARDFAIL	0x01
#define PRISM2_RESULT_NOBUFFER	0x05
#define PRISM2_RESULT_CMDERROR	0x7F

// PRISM2 BAP Offset defines
#define PRISM2_BAPOFFSET_ERROR	0x4000
#define PRISM2_BAPOFFSET_BUSY	0x8000

// PRISM2 event bit defines
#define PRISM2_EVENT_RX			0x0001
#define PRISM2_EVENT_TX			0x0002
#define PRISM2_EVENT_TXEXEC		0x0004
#define PRISM2_EVENT_ALLOC		0x0008
#define PRISM2_EVENT_CMD		0x0010
#define PRISM2_EVENT_DTIM		0x0020
#define PRISM2_EVENT_INFO		0x0080
#define PRISM2_EVENT_INFDROP	0x2000
#define PRISM2_EVENT_WTERR		0x4000
#define PRISM2_EVENT_TICK		0x8000
#define PRISM2_EVENT_ALL		0xFFFF

// PRISM2 Record ID defines (RIDs)
#define	PRISM2_RID_CNFPORTTYPE				0xFC00
#define	PRISM2_RID_CNFOWNMACADDR			0xFC01
#define	PRISM2_RID_CNFDESIREDSSID			0xFC02
#define	PRISM2_RID_CNFOWNCHANNEL			0xFC03
#define	PRISM2_RID_CNFOWNSSID				0xFC04
#define	PRISM2_RID_CNFOWNATIMWIN			0xFC05
#define	PRISM2_RID_CNFSYSSCALE				0xFC06
#define	PRISM2_RID_CNFMAXDATALEN			0xFC07
#define	PRISM2_RID_CNFWDSADDR				0xFC08
#define	PRISM2_RID_CNFPMENABLED				0xFC09
#define	PRISM2_RID_CNFPMEPS					0xFC0A
#define	PRISM2_RID_CNFMULTICASTRX			0xFC0B
#define	PRISM2_RID_CNFMAXSLEEPDUR			0xFC0C
#define	PRISM2_RID_CNFPMHOLDDUR				0xFC0D
#define	PRISM2_RID_CNFOWNNAME				0xFC0E
#define	PRISM2_RID_CNFOWNDTIMPER			0xFC10
#define	PRISM2_RID_CNFWDSADDR1				0xFC11
#define	PRISM2_RID_CNFWDSADDR2				0xFC12
#define	PRISM2_RID_CNFWDSADDR3				0xFC13
#define	PRISM2_RID_CNFWDSADDR4				0xFC14
#define	PRISM2_RID_CNFWDSADDR5				0xFC15
#define	PRISM2_RID_CNFWDSADDR6				0xFC16
#define	PRISM2_RID_CNFMCASTPMBUFF			0xFC17
#define	PRISM2_RID_CNFWEPDEFAULTKEYID		0xFC23
#define	PRISM2_RID_CNFWEPDEFAULTKEY0		0xFC24
#define	PRISM2_RID_CNFWEPDEFAULTKEY1		0xFC25
#define	PRISM2_RID_CNFWEPDEFAULTKEY2		0xFC26
#define	PRISM2_RID_CNFWEPDEFAULTKEY3		0xFC27
#define	PRISM2_RID_CNFWEPFLAGS				0xFC28
#define	PRISM2_RID_CNFWEPKEYMAPTABLE		0xFC29
#define	PRISM2_RID_CNFAUTHENTICATION		0xFC2A
#define	PRISM2_RID_CNFMAXASSOCSTATIONS		0xFC2B
#define	PRISM2_RID_CNFTXCONTROL				0xFC2C
#define	PRISM2_RID_CNFROAMINGMODE			0xFC2D
#define	PRISM2_RID_CNFHOSTAUTH				0xFC2E
#define	PRISM2_RID_CNFRCVCRCERROR			0xFC30
#define	PRISM2_RID_CNFALTRETRYCNT			0xFC32
#define	PRISM2_RID_CNFAPBCNINT				0xFC33
#define	PRISM2_RID_CNFAPPCFINFO				0xFC34
#define	PRISM2_RID_CNFSTAPCFINFO			0xFC35
#define	PRISM2_RID_CNFPRIORITYQUSAGE		0xFC37
#define	PRISM2_RID_CNFTIMCTRL				0xFC40
#define	PRISM2_RID_CNFTHIRTY2TALLY			0xFC42
#define	PRISM2_RID_CNFENHSECURITY			0xFC43
#define	PRISM2_RID_CNFDBMADJUST  			0xFC46
#define	PRISM2_RID_SSNGENERICELEMENT		0xFC48
#define	PRISM2_RID_CNFSHORTPREAMBLE			0xFCB0
#define	PRISM2_RID_CNFEXCLONGPREAMBLE		0xFCB1
#define	PRISM2_RID_CNFAUTHRSPTIMEOUT		0xFCB2
#define	PRISM2_RID_CNFBASICRATES			0xFCB3
#define	PRISM2_RID_CNFSUPPRATES				0xFCB4
#define	PRISM2_RID_CNFFALLBACKCTRL			0xFCB5
#define	PRISM2_RID_WEPKEYDISABLE  			0xFCB6
#define	PRISM2_RID_WEPKEYMAPINDEX 			0xFCB7
#define	PRISM2_RID_BROADCASTKEYID 			0xFCB8
#define	PRISM2_RID_ENTSECFLAGEYID 			0xFCB9
#define	PRISM2_RID_CNFPASSIVESCANCTRL		0xFCBA
#define	PRISM2_RID_SSNHANDLINGMODE			0xFCBB
#define	PRISM2_RID_MDCCONTROL        		0xFCBC
#define	PRISM2_RID_MDCCOUNTRY        		0xFCBD
#define	PRISM2_RID_TXPOWERMAX        		0xFCBE
#define	PRISM2_RID_CNFLFOENBLED      		0xFCBF
#define PRISM2_RID_CAPINFO					0xFCC0
#define PRISM2_RID_LISTENINTERVAL			0xFCC1
#define	PRISM2_RID_SCANREQUEST				0xFCE1
#define	PRISM2_RID_JOINREQUEST				0xFCE2
#define	PRISM2_RID_AUTHENTICATESTA			0xFCE3
#define	PRISM2_RID_CHANNELINFOREQUEST		0xFCE4
#define	PRISM2_RID_HOSTSCAN          		0xFCE5

#define PCMCIA_ATTR_COR						0x0000
#define PCMCIA_ATTR_CSR						0x0002
#define PCMCIA_ATTR_PRR						0x0004
#define PCMCIA_ATTR_SCR						0x0006
#define PCMCIA_ATTR_IOBASE0					0x0010
#define PCMCIA_ATTR_IOBASE1					0x0012
#define PCMCIA_ATTR_IOLIMIT					0x0018

// typedefs

// constants

// prototypes
#include "nic.h"


unsigned int prism2BeginPacketRetreive(void);
void prism2RetreivePacketData(u08* packet, unsigned int packetLength);
void prism2EndPacketRetreive(void);

void prism2SetupTxHeader(u16* header);
void prism2EventCheck(void);

// initialize the network interface for transmit/receive
void prism2Init(void);

void prism2GetMacAddress(u08* macaddr);
void prism2SetSSID(u08* ssid);
void prism2SetWEPKey(u08* wepkey);

u08 prism2Command(u16 cmd, u16 param0);
u08 prism2WriteBAP0(u16 id, u16 offset, u16* data, u16 len);
u08 prism2ReadBAP0(u16 id, u16 offset, u16* data, u16 len);

u08 prism2ReadRID(u16 id, u16 offset, u16* data, u16 len);
u08 prism2WriteRID(u16 id, u16 offset, u16* data, u16 len);


// hardware access commands
void prism2Write(unsigned short address, unsigned char data);
unsigned char prism2Read(unsigned short address);
void prism2Write16(unsigned short address, unsigned short data);
unsigned short prism2Read16(unsigned short address);
void prism2WriteMem(unsigned short address, unsigned short data);
unsigned short prism2ReadMem(unsigned short address);

// debugging commands
void prism2CardRegDump(void);
void prism2RegDump(void);

#endif
//@}

