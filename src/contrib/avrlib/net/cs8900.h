/*! \file cs8900.h \brief Crystal CS8900 Ethernet Interface Driver. */
//*****************************************************************************
//
// File Name	: 'cs8900.h'
// Title		: Crystal CS8900 Ethernet Interface Driver
// Author		: Pascal Stang
// Created		: 11/7/2004
// Revised		: 8/22/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
///	\ingroup network
///	\defgroup cs8900 Crystal CS8900 Ethernet Interface Driver (cs8900.c)
///	\code #include "net/cs8900.h" \endcode
///	\par Overview
///		This driver provides initialization and transmit/receive
///	functions for the Crystal CS8900 10Mb Ethernet Controller and PHY.
//
//*****************************************************************************
//@{

#ifndef CS8900_H
#define CS8900_H

#include "global.h"

#define nop()	asm volatile ("nop")


// Crystal ESIA product ID
#define CS8900_ESIA_ID				(0x630e)

// CS8900 IO Registers
#define CS8900_IO_RXTX_DATA_PORT0	(0x0000)
#define CS8900_IO_RXTX_DATA_PORT1	(0x0002)
#define CS8900_IO_TXCMD				(0x0004)
#define CS8900_IO_TXLENGTH			(0x0006)
#define CS8900_IO_ISQ				(0x0008)
#define CS8900_IO_PP_PTR			(0x000a)
#define CS8900_IO_PP_DATA_PORT0		(0x000c)
#define CS8900_IO_PP_DATA_PORT1		(0x000e)

// definitions for Crystal CS8900 ethernet-controller
// based on linux-header by Russel Nelson

#define PP_ChipID            0x0000              // offset 0h -> Corp-ID
                                                 // offset 2h -> Model/Product Number
                                                 // offset 3h -> Chip Revision Number

#define PP_ISAIOB            0x0020              // IO base address
#define PP_CS8900_ISAINT     0x0022              // ISA interrupt select
#define PP_CS8900_ISADMA     0x0024              // ISA Rec DMA channel
#define PP_ISASOF            0x0026              // ISA DMA offset
#define PP_DmaFrameCnt       0x0028              // ISA DMA Frame count
#define PP_DmaByteCnt        0x002A              // ISA DMA Byte count
#define PP_CS8900_ISAMemB    0x002C              // Memory base
#define PP_ISABootBase       0x0030              // Boot Prom base
#define PP_ISABootMask       0x0034              // Boot Prom Mask
#define PP_RxFrameByteCnt	0x0050

// EEPROM data and command registers
#define PP_EECMD             0x0040              // NVR Interface Command register
#define PP_EEData            0x0042              // NVR Interface Data Register

// Configuration and control registers
#define PP_RxCFG             0x0102              // Rx Bus config
#define PP_RxCTL             0x0104              // Receive Control Register
#define PP_TxCFG             0x0106              // Transmit Config Register
#define PP_TxCMD             0x0108              // Transmit Command Register
#define PP_BufCFG            0x010A              // Bus configuration Register
#define PP_LineCTL           0x0112              // Line Config Register
#define PP_SelfCTL           0x0114              // Self Command Register
#define PP_BusCTL            0x0116              // ISA bus control Register
#define PP_TestCTL           0x0118              // Test Register

// Status and Event Registers
#define PP_ISQ               0x0120              // Interrupt Status
#define PP_RxEvent           0x0124              // Rx Event Register
#define PP_TxEvent           0x0128              // Tx Event Register
#define PP_BufEvent          0x012C              // Bus Event Register
#define PP_RxMiss            0x0130              // Receive Miss Count
#define PP_TxCol             0x0132              // Transmit Collision Count
#define PP_LineST            0x0134              // Line State Register
#define PP_SelfST            0x0136              // Self State register
#define PP_BusST             0x0138              // Bus Status
#define PP_TDR               0x013C              // Time Domain Reflectometry

// Initiate Transmit Registers
#define PP_TxCommand         0x0144              // Tx Command
#define PP_TxLength          0x0146              // Tx Length

// Address Filter Registers
#define PP_LAF               0x0150              // Hash Table
#define PP_IA                0x0158              // Physical Address Register

// Frame Location
#define PP_RxStatus          0x0400              // Receive start of frame
#define PP_RxLength          0x0402              // Receive Length of frame
#define PP_RxFrame           0x0404              // Receive frame pointer
#define PP_TxFrame           0x0A00              // Transmit frame pointer

// Primary I/O Base Address. If no I/O base is supplied by the user, then this
// can be used as the default I/O base to access the PacketPage Area.
#define DEFAULTIOBASE				0x0300

// PP_RxCFG - Receive  Configuration and Interrupt Mask bit definition - Read/write
#define SKIP_1						0x0040
#define RX_STREAM_ENBL				0x0080
#define RX_OK_ENBL					0x0100
#define RX_DMA_ONLY					0x0200
#define AUTO_RX_DMA					0x0400
#define BUFFER_CRC					0x0800
#define RX_CRC_ERROR_ENBL			0x1000
#define RX_RUNT_ENBL				0x2000
#define RX_EXTRA_DATA_ENBL			0x4000

// PP_RxCTL - Receive Control bit definition - Read/write
#define RX_IA_HASH_ACCEPT			0x0040
#define RX_PROM_ACCEPT				0x0080
#define RX_OK_ACCEPT				0x0100
#define RX_MULTCAST_ACCEPT			0x0200
#define RX_IA_ACCEPT				0x0400
#define RX_BROADCAST_ACCEPT			0x0800
#define RX_BAD_CRC_ACCEPT			0x1000
#define RX_RUNT_ACCEPT				0x2000
#define RX_EXTRA_DATA_ACCEPT		0x4000

// PP_TxCFG - Transmit Configuration Interrupt Mask bit definition - Read/write
#define TX_LOST_CRS_ENBL			0x0040
#define TX_SQE_ERROR_ENBL			0x0080
#define TX_OK_ENBL					0x0100
#define TX_LATE_COL_ENBL			0x0200
#define TX_JBR_ENBL					0x0400
#define TX_ANY_COL_ENBL				0x0800
#define TX_16_COL_ENBL				0x8000

// PP_TxCMD - Transmit Command bit definition - Read-only and
// PP_TxCommand - Write-only
#define TX_START_5_BYTES			0x0000
#define TX_START_381_BYTES			0x0040
#define TX_START_1021_BYTES			0x0080
#define TX_START_ALL_BYTES			0x00C0
#define TX_FORCE					0x0100
#define TX_ONE_COL					0x0200
#define TX_NO_CRC					0x1000
#define TX_RUNT						0x2000

// PP_BufCFG - Buffer Configuration Interrupt Mask bit definition - Read/write
#define GENERATE_SW_INTERRUPT		0x0040
#define RX_DMA_ENBL					0x0080
#define READY_FOR_TX_ENBL			0x0100
#define TX_UNDERRUN_ENBL			0x0200
#define RX_MISS_ENBL				0x0400
#define RX_128_BYTE_ENBL			0x0800
#define TX_COL_COUNT_OVRFLOW_ENBL	0x1000
#define RX_MISS_COUNT_OVRFLOW_ENBL	0x2000
#define RX_DEST_MATCH_ENBL			0x8000

// PP_LineCTL - Line Control bit definition - Read/write
#define SERIAL_RX_ON				0x0040
#define SERIAL_TX_ON				0x0080
#define AUI_ONLY					0x0100
#define AUTO_AUI_10BASET			0x0200
#define MODIFIED_BACKOFF			0x0800
#define NO_AUTO_POLARITY			0x1000
#define TWO_PART_DEFDIS				0x2000
#define LOW_RX_SQUELCH				0x4000

// PP_SelfCTL - Software Self Control bit definition - Read/write
#define POWER_ON_RESET				0x0040
#define SW_STOP						0x0100
#define SLEEP_ON					0x0200
#define AUTO_WAKEUP					0x0400
#define HCB0_ENBL					0x1000
#define HCB1_ENBL					0x2000
#define HCB0						0x4000
#define HCB1						0x8000

// PP_BusCTL - ISA Bus Control bit definition - Read/write
#define RESET_RX_DMA				0x0040
#define MEMORY_ON					0x0400
#define DMA_BURST_MODE				0x0800
#define IO_CHANNEL_READY_ON			0x1000
#define RX_DMA_SIZE_64K				0x2000
#define ENABLE_IRQ					0x8000

// PP_TestCTL - Test Control bit definition - Read/write
#define LINK_OFF             0x0080
#define ENDEC_LOOPBACK       0x0200
#define AUI_LOOPBACK         0x0400
#define BACKOFF_OFF          0x0800
#define FDX_8900             0x4000

// PP_RxEvent - Receive Event Bit definition - Read-only
#define RX_IA_HASHED         0x0040
#define RX_DRIBBLE           0x0080
#define RX_OK                0x0100
#define RX_HASHED            0x0200
#define RX_IA                0x0400
#define RX_BROADCAST         0x0800
#define RX_CRC_ERROR         0x1000
#define RX_RUNT              0x2000
#define RX_EXTRA_DATA        0x4000
#define HASH_INDEX_MASK      0xFC00              // Hash-Table Index Mask (6 Bit)

// PP_TxEvent - Transmit Event Bit definition - Read-only
#define TX_LOST_CRS          0x0040
#define TX_SQE_ERROR         0x0080
#define TX_OK                0x0100
#define TX_LATE_COL          0x0200
#define TX_JBR               0x0400
#define TX_16_COL            0x8000
#define TX_COL_COUNT_MASK    0x7800

// PP_BufEvent - Buffer Event Bit definition - Read-only
#define SW_INTERRUPT         0x0040
#define RX_DMA               0x0080
#define READY_FOR_TX         0x0100
#define TX_UNDERRUN          0x0200
#define RX_MISS              0x0400
#define RX_128_BYTE          0x0800
#define TX_COL_OVRFLW        0x1000
#define RX_MISS_OVRFLW       0x2000
#define RX_DEST_MATCH        0x8000

// PP_LineST - Ethernet Line Status bit definition - Read-only
#define LINK_OK              0x0080
#define AUI_ON               0x0100
#define TENBASET_ON          0x0200
#define POLARITY_OK          0x1000
#define CRS_OK               0x4000

// PP_SelfST - Chip Software Status bit definition
#define ACTIVE_33V           0x0040
#define INIT_DONE            0x0080
#define SI_BUSY              0x0100
#define EEPROM_PRESENT       0x0200
#define EEPROM_OK            0x0400
#define EL_PRESENT           0x0800
#define EE_SIZE_64           0x1000

// PP_BusST - ISA Bus Status bit definition
#define TX_BID_ERROR         0x0080
#define READY_FOR_TX_NOW     0x0100

// The following block defines the ISQ event types
#define ISQ_RX_EVENT         0x0004
#define ISQ_TX_EVENT         0x0008
#define ISQ_BUFFER_EVENT     0x000C
#define ISQ_RX_MISS_EVENT    0x0010
#define ISQ_TX_COL_EVENT     0x0012

#define ISQ_EVENT_MASK       0x003F              // ISQ mask to find out type of event

#define AUTOINCREMENT        0x8000              // Bit mask to set Bit-15 for autoincrement

// EEProm Commands
#define EEPROM_WRITE_EN      0x00F0
#define EEPROM_WRITE_DIS     0x0000
#define EEPROM_WRITE_CMD     0x0100
#define EEPROM_READ_CMD      0x0200

// Receive Header of each packet in receive area of memory for DMA-Mode
#define RBUF_EVENT_LOW       0x0000              // Low byte of RxEvent
#define RBUF_EVENT_HIGH      0x0001              // High byte of RxEvent
#define RBUF_LEN_LOW         0x0002              // Length of received data - low byte
#define RBUF_LEN_HI          0x0003              // Length of received data - high byte
#define RBUF_HEAD_LEN        0x0004              // Length of this header

// typedefs

// constants

// prototypes

#include "nic.h"

unsigned int cs8900BeginPacketRetreive(void);
void cs8900RetreivePacketData(u08* packet, unsigned int packetLength);
void cs8900EndPacketRetreive(void);


void cs8900Init(void);
void cs8900Write(unsigned char address, unsigned char data);
unsigned char cs8900Read(unsigned char address);

void cs8900Write16(unsigned char address, unsigned short data);
unsigned short cs8900Read16(unsigned char address);

void cs8900WriteReg(unsigned short address, unsigned short data);
unsigned short cs8900ReadReg(unsigned short address);

void cs8900CopyToFrame(unsigned char *source, unsigned short size);
void cs8900CopyFromFrame(unsigned char *dest, unsigned short size);

u08 cs8900LinkStatus(void);

void cs8900IORegDump(void);
void cs8900RegDump(void);

#endif
//@}


/****************

// CS8900 device register definitions

// Crystal ESIA product id.

#define CS8900_ESIA_ID             (0x630e)

//IO Registers.
#define CS8900_IO_RX_TX_DATA_PORT0 (0x0000)
#define CS8900_IO_TX_TX_DATA_PORT1 (0x0002)
#define CS8900_IO_TxCMD            (0x0004)
#define CS8900_IO_TxLength         (0x0006)
#define CS8900_IO_ISQ              (0x0008)
#define CS8900_IO_PACKET_PAGE_PTR  (0x000a)
#define CS8900_IO_PP_DATA_PORT0    (0x000c)
#define CS8900_IO_PP_DATA_PORT1    (0x000e)

 * Packet Page Registers.

 * Bus Interface Registers.

#define CS8900_PP_PROD_ID          (0x0000)
#define CS8900_PP_IO_BASE          (0x0020)
#define CS8900_PP_INT              (0x0022)
#define CS8900_PP_DMA_CHANNEL      (0x0024)
#define CS8900_PP_DMA_SOF          (0x0026)
#define CS8900_PP_DMA_FRM_CNT      (0x0028)
#define CS8900_PP_DMA_RX_BCNT      (0x002a)
#define CS8900_PP_MEM_BASE         (0x002c)
#define CS8900_PP_BPROM_BASE       (0x0030)
#define CS8900_PP_BPROM_AMASK      (0x0034)
#define CS8900_PP_EEPROM_CMD       (0x0040)
#define CS8900_PP_EEPROM_DATA      (0x0042)
#define CS8900_PP_RX_FRAME_BCNT    (0x0050)

 * Configuration and Control Registers.

#define CS8900_PP_RxCFG            (0x0102)
#define CS8900_PP_RxCTL            (0x0104)
#define CS8900_PP_TxCFG            (0x0106)
#define CS8900_PP_TxCMD_READ       (0x0108)
#define CS8900_PP_BufCFG           (0x010a)
#define CS8900_PP_LineCFG          (0x0112)
#define CS8900_PP_SelfCTL          (0x0114)
#define CS8900_PP_BusCTL           (0x0116)
#define CS8900_PP_TestCTL          (0x0118)

 * Status and Event Registers.

#define CS8900_PP_ISQ              (0x0120)
#define CS8900_PP_RxEvent          (0x0124)
#define CS8900_PP_TxEvent          (0x0128)
#define CS8900_PP_BufEvent         (0x012c)
#define CS8900_PP_RxMISS           (0x0130)
#define CS8900_PP_TxCol            (0x0132)
#define CS8900_PP_LineST           (0x0134)
#define CS8900_PP_SelfST           (0x0136)
#define CS8900_PP_BusST            (0x0138)
#define CS8900_PP_TDR              (0x013c)

 * Initiate Transmit Registers.
#define CS8900_PP_TxCMD            (0x0144)
#define CS8900_PP_TxLength         (0x0146)

* Address Filter Registers.
#define CS8900_PP_LAF              (0x0150)
#define CS8900_PP_IA               (0x0158)

 * Frame Location.
#define CS8900_PP_RxStatus         (0x0400)
#define CS8900_PP_RxLength         (0x0402)
#define CS8900_PP_RxFrameLoc       (0x0404)
#define CS8900_PP_TxFrameLoc       (0x0a00)

 * Bit Definitions of Registers.
 * IO Packet Page Pointer.
#define CS8900_PPP_AUTO_INCREMENT             (0x8000)

 * Reg 3. Receiver Configuration.
#define CS8900_RX_CONFIG_SKIP_1               (1 << 6)
#define CS8900_RX_CONFIG_STREAM_ENABLE        (1 << 7)
#define CS8900_RX_CONFIG_RX_OK                (1 << 8)
#define CS8900_RX_CONFIG_RX_DMA               (1 << 9)
#define CS8900_RX_CONFIG_RX_AUTO_DMA          (1 << 10)
#define CS8900_RX_CONFIG_BUFFER_CRC           (1 << 11)
#define CS8900_RX_CONFIG_CRC_ERROR            (1 << 12)
#define CS8900_RX_CONFIG_RUNT                 (1 << 13)
#define CS8900_RX_CONFIG_EXTRA_DATA           (1 << 14)

 * Reg 4. Receiver Event.
#define CS8900_RX_EVENT_HASH_IA_MATCH         (1 << 6)
#define CS8900_RX_EVENT_DRIBBLE_BITS          (1 << 7)
#define CS8900_RX_EVENT_RX_OK                 (1 << 8)
#define CS8900_RX_EVENT_HASHED                (1 << 9)
#define CS8900_RX_EVENT_IA                    (1 << 10)
#define CS8900_RX_EVENT_BROADCAST             (1 << 11)
#define CS8900_RX_EVENT_CRC_ERROR             (1 << 12)
#define CS8900_RX_EVENT_RUNT                  (1 << 13)
#define CS8900_RX_EVENT_EXTRA_DATA            (1 << 14)

 * Reg 5. Receiver Control.
#define CS8900_RX_CTRL_HASH_IA_MATCH          (1 << 6)
#define CS8900_RX_CTRL_PROMISCUOUS            (1 << 7)
#define CS8900_RX_CTRL_RX_OK                  (1 << 8)
#define CS8900_RX_CTRL_MULTICAST              (1 << 9)
#define CS8900_RX_CTRL_INDIVIDUAL             (1 << 10)
#define CS8900_RX_CTRL_BROADCAST              (1 << 11)
#define CS8900_RX_CTRL_CRC_ERROR              (1 << 12)
#define CS8900_RX_CTRL_RUNT                   (1 << 13)
#define CS8900_RX_CTRL_EXTRA_DATA             (1 << 14)

 * Reg 7. Transmit Configuration.
#define CS8900_TX_CONFIG_LOSS_OF_CARRIER      (1 << 6)
#define CS8900_TX_CONFIG_SQ_ERROR             (1 << 7)
#define CS8900_TX_CONFIG_TX_OK                (1 << 8)
#define CS8900_TX_CONFIG_OUT_OF_WINDOW        (1 << 9)
#define CS8900_TX_CONFIG_JABBER               (1 << 10)
#define CS8900_TX_CONFIG_ANY_COLLISION        (1 << 11)
#define CS8900_TX_CONFIG_16_COLLISION         (1 << 15)

 * Reg 8. Transmit Event.
#define CS8900_TX_EVENT_LOSS_OF_CARRIER       (1 << 6)
#define CS8900_TX_EVENT_SQ_ERROR              (1 << 7)
#define CS8900_TX_EVENT_TX_OK                 (1 << 8)
#define CS8900_TX_EVENT_OUT_OF_WINDOW         (1 << 9)
#define CS8900_TX_EVENT_JABBER                (1 << 10)
#define CS8900_TX_EVENT_16_COLLISIONS         (1 << 15)

 * Reg 9. Transmit Command Status.
#define CS8900_TX_CMD_STATUS_TX_START_5       (0 << 6)
#define CS8900_TX_CMD_STATUS_TX_START_381     (1 << 6)
#define CS8900_TX_CMD_STATUS_TX_START_1021    (2 << 6)
#define CS8900_TX_CMD_STATUS_TX_START_ENTIRE  (3 << 6)
#define CS8900_TX_CMD_STATUS_FORCE            (1 << 8)
#define CS8900_TX_CMD_STATUS_ONE_COLLISION    (1 << 9)
#define CS8900_TX_CMD_STATUS_INHIBIT_CRC      (1 << 12)
#define CS8900_TX_CMD_STATUS_TX_PAD_DISABLED  (1 << 13)

 * Reg B. Buffer Configuration.
#define CS8900_BUFFER_CONFIG_SW_INT           (1 << 6)
#define CS8900_BUFFER_CONFIG_RX_DMA_DONE      (1 << 7)
#define CS8900_BUFFER_CONFIG_RDY_FOR_TX       (1 << 8)
#define CS8900_BUFFER_CONFIG_TX_UNDERRUN      (1 << 9)
#define CS8900_BUFFER_CONFIG_RX_MISSED        (1 << 10)
#define CS8900_BUFFER_CONFIG_RX_128_BYTES     (1 << 11)
#define CS8900_BUFFER_CONFIG_TX_COL_OVF       (1 << 12)
#define CS8900_BUFFER_CONFIG_RX_MISSED_OVF    (1 << 13)
#define CS8900_BUFFER_CONFIG_RX_DEST_MATCH    (1 << 15)

 * Reg C. Buffer Event.
#define CS8900_BUFFER_EVENT_SW_INT            (1 << 6)
#define CS8900_BUFFER_EVENT_RX_DMA_DONE       (1 << 7)
#define CS8900_BUFFER_EVENT_RDY_FOR_TX        (1 << 8)
#define CS8900_BUFFER_EVENT_TX_UNDERRUN       (1 << 9)
#define CS8900_BUFFER_EVENT_RX_MISSED         (1 << 10)
#define CS8900_BUFFER_EVENT_RX_128_BYTES      (1 << 11)
#define CS8900_BUFFER_EVENT_RX_DEST_MATCH     (1 << 15)

 * Reg 13. Line Control.
#define CS8900_LINE_CTRL_RX_ON               (1 << 6)
#define CS8900_LINE_CTRL_TX_ON               (1 << 7)
#define CS8900_LINE_CTRL_AUI                 (1 << 8)
#define CS8900_LINE_CTRL_10BASET             (0 << 9)
#define CS8900_LINE_CTRL_AUTO_AUI_10BASET    (1 << 9)
#define CS8900_LINE_CTRL_MOD_BACKOFF         (1 << 11)
#define CS8900_LINE_CTRL_POLARITY_DISABLED   (1 << 12)
#define CS8900_LINE_CTRL_2_PART_DEF_DISABLED (1 << 13)
#define CS8900_LINE_CTRL_LO_RX_SQUELCH       (1 << 14)

 * Reg 14. Line Status.
#define CS8900_LINE_STATUS_LINK_OK           (1 << 7)
#define CS8900_LINE_STATUS_AUI               (1 << 8)
#define CS8900_LINE_STATUS_10_BASE_T         (1 << 9)
#define CS8900_LINE_STATUS_POLARITY_OK       (1 << 12)
#define CS8900_LINE_STATUS_CRS               (1 << 14)

 * Reg 15. Self Control.
#define CS8900_SELF_CTRL_RESET              (1 << 6)
#define CS8900_SELF_CTRL_SW_SUSPEND         (1 << 8)
#define CS8900_SELF_CTRL_HW_SLEEP           (1 << 9)
#define CS8900_SELF_CTRL_HW_STANDBY         (1 << 10)
#define CS8900_SELF_CTRL_HC0E               (1 << 12)
#define CS8900_SELF_CTRL_HC1E               (1 << 13)
#define CS8900_SELF_CTRL_HCB0               (1 << 14)
#define CS8900_SELF_CTRL_HCB1               (1 << 15)

 * Reg 16. Self Status.
#define CS8900_SELF_STATUS_3_3_V            (1 << 6)
#define CS8900_SELF_STATUS_INITD            (1 << 7)
#define CS8900_SELF_STATUS_SIBUST           (1 << 8)
#define CS8900_SELF_STATUS_EEPROM_PRESENT   (1 << 9)  
#define CS8900_SELF_STATUS_EEPROM_OK        (1 << 10)
#define CS8900_SELF_STATUS_EL_PRESENT       (1 << 11)
#define CS8900_SELF_STATUS_EE_SIZE          (1 << 12)

 * Reg 17. Bus Control.
#define CS8900_BUS_CTRL_RESET_RX_DMA        (1 << 6)
#define CS8900_BUS_CTRL_USE_SA              (1 << 9)
#define CS8900_BUS_CTRL_MEMORY_ENABLE       (1 << 10)
#define CS8900_BUS_CTRL_DMA_BURST           (1 << 11)
#define CS8900_BUS_CTRL_IOCHRDYE            (1 << 12)
#define CS8900_BUS_CTRL_RX_DMA_SIZE         (1 << 13)
#define CS8900_BUS_CTRL_ENABLE_INT          (1 << 15)

 * Reg 18. Bus Status.
#define CS8900_BUS_STATUS_TX_BID_ERROR      (1 << 7)
#define CS8900_BUS_STATUS_RDY_FOR_TX_NOW    (1 << 8)

*/
