/*--------------------------------------------------------------------------
 * reg24lu1.h
 *
 * Keil C51 header file for the Nordic Semiconductor nRF24LU1 2.4GHz RF
 * transceiver with embedded 8051 compatible microcontroller and USB.
 * Converted to SDCC via ./scripts/keil2sdcc
 *
 *
 *------------------------------------------------------------------------*/
#ifndef __REG24LU1_H__
#define __REG24LU1_H__

//-----------------------------------------------------------------------------
// Byte Registers
//-----------------------------------------------------------------------------

__sfr __at(0x80) P0;
#define __SFR_P0 0x80
__sfr __at(0x81) SP;
#define __SFR_SP 0x81
__sfr __at(0x82) DPL;
#define __SFR_DPL 0x82
__sfr __at(0x83) DPH;
#define __SFR_DPH 0x83
__sfr __at(0x84) DPL1;
#define __SFR_DPL1 0x84
__sfr __at(0x85) DPH1;
#define __SFR_DPH1 0x85
__sfr __at(0x87) PCON;
#define __SFR_PCON 0x87
__sfr __at(0x88) TCON;
#define __SFR_TCON 0x88
__sfr __at(0x89) TMOD;
#define __SFR_TMOD 0x89
__sfr __at(0x8A) TL0;
#define __SFR_TL0 0x8A
__sfr __at(0x8B) TL1;
#define __SFR_TL1 0x8B
__sfr __at(0x8C) TH0;
#define __SFR_TH0 0x8C
__sfr __at(0x8D) TH1;
#define __SFR_TH1 0x8D
__sfr __at(0x8E) CKCON;
#define __SFR_CKCON 0x8E
__sfr __at(0x90) RFCON;
#define __SFR_RFCON 0x90
__sfr __at(0x92) DPS;
#define __SFR_DPS 0x92
__sfr __at(0x94) P0DIR;
#define __SFR_P0DIR 0x94
__sfr __at(0x95) P0ALT;
#define __SFR_P0ALT 0x95
__sfr __at(0x98) S0CON;
#define __SFR_S0CON 0x98
__sfr __at(0x99) S0BUF;
#define __SFR_S0BUF 0x99
__sfr __at(0x9A) IEN2;
#define __SFR_IEN2 0x9A
__sfr __at(0xA0) USBCON;
#define __SFR_USBCON 0xA0
__sfr __at(0xA3) CLKCTL;
#define __SFR_CLKCTL 0xA3
__sfr __at(0xA4) PWRDWN;
#define __SFR_PWRDWN 0xA4
__sfr __at(0xA5) WUCONF;
#define __SFR_WUCONF 0xA5
__sfr __at(0xA6) INTEXP;
#define __SFR_INTEXP 0xA6
__sfr __at(0xA8) IEN0;
#define __SFR_IEN0 0xA8
__sfr __at(0xA9) IP0;
#define __SFR_IP0 0xA9
__sfr __at(0xAA) S0RELL;
#define __SFR_S0RELL 0xAA
__sfr __at(0xAB) REGXH;
#define __SFR_REGXH 0xAB
__sfr __at(0xAC) REGXL;
#define __SFR_REGXL 0xAC
__sfr __at(0xAD) REGXC;
#define __SFR_REGXC 0xAD
__sfr __at(0xB1) RSTRES;
#define __SFR_RSTRES 0xB1
__sfr __at(0xB2) SMDAT;
#define __SFR_SMDAT 0xB2
__sfr __at(0xB3) SMCTL;
#define __SFR_SMCTL 0xB3
__sfr __at(0xB5) TICKDV;
#define __SFR_TICKDV 0xB5
__sfr __at(0xB8) IEN1;
#define __SFR_IEN1 0xB8
__sfr __at(0xB9) IP1;
#define __SFR_IP1 0xB9
__sfr __at(0xBA) S0RELH;
#define __SFR_S0RELH 0xBA
__sfr __at(0xBC) SSCONF;
#define __SFR_SSCONF 0xBC
__sfr __at(0xBD) SSDATA;
#define __SFR_SSDATA 0xBD
__sfr __at(0xBE) SSSTAT;
#define __SFR_SSSTAT 0xBE
__sfr __at(0xC0) IRCON;
#define __SFR_IRCON 0xC0
__sfr __at(0xC1) CCEN;
#define __SFR_CCEN 0xC1
__sfr __at(0xC2) CCL1;
#define __SFR_CCL1 0xC2
__sfr __at(0xC3) CCH1;
#define __SFR_CCH1 0xC3
__sfr __at(0xC4) CCL2;
#define __SFR_CCL2 0xC4
__sfr __at(0xC5) CCH2;
#define __SFR_CCH2 0xC5
__sfr __at(0xC6) CCL3;
#define __SFR_CCL3 0xC6
__sfr __at(0xC7) CCH3;
#define __SFR_CCH3 0xC7
__sfr __at(0xC8) T2CON;
#define __SFR_T2CON 0xC8
__sfr __at(0xC9) P0EXP;
#define __SFR_P0EXP 0xC9
__sfr __at(0xCA) CRCL;
#define __SFR_CRCL 0xCA
__sfr __at(0xCB) CRCH;
#define __SFR_CRCH 0xCB
__sfr __at(0xCC) TL2;
#define __SFR_TL2 0xCC
__sfr __at(0xCD) TH2;
#define __SFR_TH2 0xCD
__sfr __at(0xD0) PSW;
#define __SFR_PSW 0xD0
__sfr __at(0xD8) WDCON;
#define __SFR_WDCON 0xD8
__sfr __at(0xD9) USBSLP;
#define __SFR_USBSLP 0xD9
__sfr __at(0xE0) ACC;
#define __SFR_ACC 0xE0
__sfr __at(0xE5) RFDAT;
#define __SFR_RFDAT 0xE5
__sfr __at(0xE6) RFCTL;
#define __SFR_RFCTL 0xE6
__sfr __at(0xE8) AESCS;
#define __SFR_AESCS 0xE8
__sfr __at(0xE9) MD0;
#define __SFR_MD0 0xE9
__sfr __at(0xEA) MD1;
#define __SFR_MD1 0xEA
__sfr __at(0xEB) MD2;
#define __SFR_MD2 0xEB
__sfr __at(0xEC) MD3;
#define __SFR_MD3 0xEC
__sfr __at(0xED) MD4;
#define __SFR_MD4 0xED
__sfr __at(0xEE) MD5;
#define __SFR_MD5 0xEE
__sfr __at(0xEF) ARCON;
#define __SFR_ARCON 0xEF
__sfr __at(0xF0) B;
#define __SFR_B 0xF0
__sfr __at(0xF1) AESKIN;
#define __SFR_AESKIN 0xF1
__sfr __at(0xF2) AESIV;
#define __SFR_AESIV 0xF2
__sfr __at(0xF3) AESD;
#define __SFR_AESD 0xF3
__sfr __at(0xF5) AESIA1;
#define __SFR_AESIA1 0xF5
__sfr __at(0xF6) AESIA2;
#define __SFR_AESIA2 0xF6
__sfr __at(0xF8) FSR;
#define __SFR_FSR 0xF8
__sfr __at(0xF9) FPCR;
#define __SFR_FPCR 0xF9
__sfr __at(0xFA) FCR;
#define __SFR_FCR 0xFA

//-----------------------------------------------------------------------------
// Word Registers
//-----------------------------------------------------------------------------

__sfr16 __at(0xC2) CC1;
#define __SFR16_CC1 0xC2
__sfr16 __at(0xC4) CC2;
#define __SFR16_CC2 0xC4
__sfr16 __at(0xC6) CC3;
#define __SFR16_CC3 0xC6
__sfr16 __at(0xCA) CRC;
#define __SFR16_CRC 0xCA
__sfr16 __at(0xCC) T2;
#define __SFR16_T2 0xCC

//-----------------------------------------------------------------------------
/* Lint uses a trick (see co-kc51.lnt) where sbit gets treated like (expanded to) a bool.
   This causes errors due to the strong type checking of _Bool (and thereby bool) that
   is turned on in our implementation of stdbool.h.
   Therefore, we suppress Lint warning 18 ("Redeclaration") for the sbit-s declared in this file.
*/

/*lint -e18 */
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

/*  FSR  */
__sbit __at(__SFR_FSR^7) MCDIS;
__sbit __at(__SFR_FSR^6) STP;
__sbit __at(__SFR_FSR^5) WEN;
__sbit __at(__SFR_FSR^4) RDYN;
__sbit __at(__SFR_FSR^3) INFEN;
__sbit __at(__SFR_FSR^2) RDIS;
__sbit __at(__SFR_FSR^1) RDEND;
__sbit __at(__SFR_FSR^0) WPEN;

/*  PSW   */
__sbit __at(__SFR_PSW^7) CY;
__sbit __at(__SFR_PSW^6) AC;
__sbit __at(__SFR_PSW^5) F0;
__sbit __at(__SFR_PSW^4) RS1;
__sbit __at(__SFR_PSW^3) RS0;
__sbit __at(__SFR_PSW^2) OV;
__sbit __at(__SFR_PSW^1) F1;
__sbit __at(__SFR_PSW^0) P;

/*  TCON  */
__sbit __at(__SFR_TCON^7) TF1;
__sbit __at(__SFR_TCON^6) TR1;
__sbit __at(__SFR_TCON^5) TF0;
__sbit __at(__SFR_TCON^4) TR0;
__sbit __at(__SFR_TCON^3) IE1;
__sbit __at(__SFR_TCON^2) IT1;
__sbit __at(__SFR_TCON^1) IE0;
__sbit __at(__SFR_TCON^0) IT0;

/*  S0CON  */
__sbit __at(__SFR_S0CON^7) SM0;
__sbit __at(__SFR_S0CON^6) SM1;
__sbit __at(__SFR_S0CON^5) SM20;
__sbit __at(__SFR_S0CON^4) REN0;
__sbit __at(__SFR_S0CON^3) TB80;
__sbit __at(__SFR_S0CON^2) RB80;
__sbit __at(__SFR_S0CON^1) TI0;
__sbit __at(__SFR_S0CON^0) RI0;

/*  T2CON  */
__sbit __at(__SFR_T2CON^7) T2PS;
__sbit __at(__SFR_T2CON^6) I3FR;
__sbit __at(__SFR_T2CON^5) I2FR;
__sbit __at(__SFR_T2CON^4) T2R1;
__sbit __at(__SFR_T2CON^3) T2R0;
__sbit __at(__SFR_T2CON^2) T2CM;
__sbit __at(__SFR_T2CON^1) T2I1;
__sbit __at(__SFR_T2CON^0) T2I0;

/*  IEN0  */
__sbit __at(__SFR_IEN0^7) EA;

__sbit __at(__SFR_IEN0^5) ET2;
__sbit __at(__SFR_IEN0^4) ES0;
__sbit __at(__SFR_IEN0^3) ET1;
__sbit __at(__SFR_IEN0^2) EX1;
__sbit __at(__SFR_IEN0^1) ET0;
__sbit __at(__SFR_IEN0^0) EX0;

/* IEN1  */
__sbit __at(__SFR_IEN1^7) EXEN2;

__sbit __at(__SFR_IEN1^5) WUIRQ;
__sbit __at(__SFR_IEN1^4) USB;
__sbit __at(__SFR_IEN1^3) USBWU;
__sbit __at(__SFR_IEN1^2) SPI;
__sbit __at(__SFR_IEN1^1) RF;
__sbit __at(__SFR_IEN1^0) RFSPI;

/* IRCON */
__sbit __at(__SFR_IRCON^7) EXF2;
__sbit __at(__SFR_IRCON^6) TF2;
__sbit __at(__SFR_IRCON^5) WUF;
__sbit __at(__SFR_IRCON^4) USBF;
__sbit __at(__SFR_IRCON^3) USBWUF;
__sbit __at(__SFR_IRCON^2) SPIF;
__sbit __at(__SFR_IRCON^1) RFF;
__sbit __at(__SFR_IRCON^0) RFSPIF;

/* USBCON */
__sbit __at(__SFR_USBCON^7) SWRST;
__sbit __at(__SFR_USBCON^6) WU;
__sbit __at(__SFR_USBCON^5) SUSPEND;
__sbit __at(__SFR_USBCON^4) IV4;
__sbit __at(__SFR_USBCON^3) IV3;
__sbit __at(__SFR_USBCON^2) IV2;
__sbit __at(__SFR_USBCON^1) IV1;
__sbit __at(__SFR_USBCON^0) IV0;

/* PORT0 */
__sbit __at(__SFR_P0^0) P00;
__sbit __at(__SFR_P0^1) P01;
__sbit __at(__SFR_P0^2) P02;
__sbit __at(__SFR_P0^3) P03;
__sbit __at(__SFR_P0^3) MCSN;
__sbit __at(__SFR_P0^3) SCSN;
__sbit __at(__SFR_P0^4) P04;
__sbit __at(__SFR_P0^5) P05;

/* RFCON */
__sbit __at(__SFR_RFCON^0) RFCE;
__sbit __at(__SFR_RFCON^1) RFCSN;
__sbit __at(__SFR_RFCON^2) RFCKEN;

/* WDCON */
__sbit __at(__SFR_WDCON^7) BD;

/* AESCS */
__sbit __at(__SFR_AESCS^0) GO;
__sbit __at(__SFR_AESCS^1) DECR;

/*lint +e18 */  /* Re-enable Lint warning 18 */


/* REGX commands */
#define RWD         0x00
#define WWD         0x08
#define RGTIMER     0x01
#define WGTIMER     0x09
#define RRTCLAT     0x02
#define WRTCLAT     0x0A
#define RRTC        0x03
#define WRTCDIS     0x0B
#define RWSTA0      0x04
#define WWCON0      0x0C
#define RWSTA1      0x05
#define WWCON1      0x0D

//-----------------------------------------------------------------------------
// Interrupt Vector Definitions
//-----------------------------------------------------------------------------

#define INTERRUPT_EXT_INT0     0   // External Interrupt0 (P0.3)
#define INTERRUPT_T0           1   // Timer0 Overflow
#define INTERRUPT_AES_RDY      2   // AES ready interrupt
#define INTERRUPT_T1           3   // Timer1 Overflow
#define INTERRUPT_UART0        4   // UART0, Receive & Transmitt interrupt
#define INTERRUPT_T2           5   // Timer2 Overflow
#define INTERRUPT_RF_RDY       8   // RF SPI ready interrupt
#define INTERRUPT_RFIRQ        9   // RF interrupt
#define INTERRUPT_SPI          10  // SPI interrupt
#define INTERRUPT_USB_WU       11  // USB wakeup interrupt
#define INTERRUPT_USB_INT      12  // USB interrupt
#define INTERRUPT_WU           13  // Internal wakeup interrupt

//-----------------------------------------------------------------------------
// USB STUFF
//-----------------------------------------------------------------------------


//XDATA (BIG) Part (Partial definition. Lack ISO registers)
#define IVEC    (*((__xdata unsigned char *)0xC7A8))
#define IN_IRQ  (*((__xdata unsigned char *)0xC7A9))
#define OUT_IRQ (*((__xdata unsigned char *)0xC7AA))
#define USBIRQ  (*((__xdata unsigned char *)0xC7AB))
#define IN_IEN  (*((__xdata unsigned char *)0xC7AC))
#define OUT_IEN (*((__xdata unsigned char *)0xC7AD))
#define USBIEN  (*((__xdata unsigned char *)0xC7AE))

#define EP0CS   (*((__xdata unsigned char *)0xC7B4))

#define IN0BC   (*((__xdata unsigned char *)0xC7B5))
#define IN1CS   (*((__xdata unsigned char *)0xC7B6))
#define IN1BC   (*((__xdata unsigned char *)0xC7B7))
#define IN2CS   (*((__xdata unsigned char *)0xC7B8))
#define IN2BC   (*((__xdata unsigned char *)0xC7B9))
#define IN3CS   (*((__xdata unsigned char *)0xC7BA))
#define IN3BC   (*((__xdata unsigned char *)0xC7BB))
#define IN4CS   (*((__xdata unsigned char *)0xC7BC))
#define IN4BC   (*((__xdata unsigned char *)0xC7BD))
#define IN5CS   (*((__xdata unsigned char *)0xC7BE))
#define IN5BC   (*((__xdata unsigned char *)0xC7BF))

#define OUT0BC   (*((__xdata unsigned char *)0xC7C5))
#define OUT1CS   (*((__xdata unsigned char *)0xC7C6))
#define OUT1BC   (*((__xdata unsigned char *)0xC7C7))
#define OUT2CS   (*((__xdata unsigned char *)0xC7C8))
#define OUT2BC   (*((__xdata unsigned char *)0xC7C9))
#define OUT3CS   (*((__xdata unsigned char *)0xC7CA))
#define OUT3BC   (*((__xdata unsigned char *)0xC7CB))
#define OUT4CS   (*((__xdata unsigned char *)0xC7CC))
#define OUT4BC   (*((__xdata unsigned char *)0xC7CD))
#define OUT5CS   (*((__xdata unsigned char *)0xC7CE))
#define OUT5BC   (*((__xdata unsigned char *)0xC7CF))


//EP Address pointers
#define BOUT1ADDR (*((__xdata unsigned char *)0xC781))
#define BOUT2ADDR (*((__xdata unsigned char *)0xC782))
#define BOUT3ADDR (*((__xdata unsigned char *)0xC783))
#define BOUT4ADDR (*((__xdata unsigned char *)0xC784))
#define BOUT5ADDR (*((__xdata unsigned char *)0xC785))
#define BINSTADDR (*((__xdata unsigned char *)0xC788))
#define BIN1ADDR  (*((__xdata unsigned char *)0xC789))
#define BIN2ADDR  (*((__xdata unsigned char *)0xC78A))
#define BIN3ADDR  (*((__xdata unsigned char *)0xC78B))
#define BIN4ADDR  (*((__xdata unsigned char *)0xC78C))
#define BIN5ADDR  (*((__xdata unsigned char *)0xC78D))

#define USBCS      (*((__xdata unsigned char *)0xC7D6))
#define TOGCTL     (*((__xdata unsigned char *)0xC7D7))
#define USBFRAME   (*((__xdata unsigned short*)0xC7D8))
#define FNADDR     (*((__xdata unsigned char *)0xC7DB))
#define USBPAIR    (*((__xdata unsigned char *)0xC7DD))
#define INBULKVAL  (*((__xdata unsigned char *)0xC7DE))
#define OUTBULKVAL (*((__xdata unsigned char *)0xC7DF))

#define INISOVAL (*((__xdata unsigned char *)0xC7E0))
#define OUTISOVAL (*((__xdata unsigned char *)0xC700))

static volatile unsigned char __at (0xC7E8) SETUPBUF[8];

//EP data buffers
static volatile unsigned char __at (0xC6C0) OUT0BUF[64];
static volatile unsigned char __at (0xC700) IN0BUF[64];
static volatile unsigned char __at (0xC640) OUT1BUF[64];
static volatile unsigned char __at (0xC680) IN1BUF[64];
static volatile unsigned char __at (0xC5C0) OUT2BUF[64];
static volatile unsigned char __at (0xC600) IN2BUF[64];
static volatile unsigned char __at (0xC540) OUT3BUF[64];
static volatile unsigned char __at (0xC580) IN3BUF[64];
static volatile unsigned char __at (0xC4C0) OUT4BUF[64];
static volatile unsigned char __at (0xC500) IN4BUF[64];
static volatile unsigned char __at (0xC440) OUT5BUF[64];
static volatile unsigned char __at (0xC480) IN5BUF[64];

#endif
