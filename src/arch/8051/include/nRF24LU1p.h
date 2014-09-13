/**
 *    ||        ____  _ __  ______
 * +------+    / __ )(_) /_/  ____/_________ ____  ____
 * | 0xBC |   / __ /  / __/ /    / ___/ __ `/_  / / _  \
 * +------+  / /_/ / / /_/ /___ / /  / /_/ / / /_/   __/
 *  ||  ||  /_____/_/\__/\____//_/   \__,_/ /___/ \___/
 *
 * CrazyRadio firmware
 *
 * Copyright (C) 2011-2013 BitCraze AB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, in version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * nRF24LU1P.h - Cpu SFR definitions
 */

#ifndef __NRF24LU1P_H__
#define __NRF24LU1P_H__

//Radio SPI regs
__sfr __at (0xE5) RFDAT;
__sfr __at (0xE6) RFCTL;

__sfr  __at (0x90) RFCON;
__sbit __at (0x90) RFCE;
__sbit __at (0x91) RFCSN;
__sbit __at (0x92) RFCKEN;


//USB regs
__sfr __at (0xA0) USBCON;
__sfr __at (0xD9) USBSLP;

//PIO
__sfr  __at (0x80) P0;
__sbit __at (0x80) P0_0;
__sbit __at (0x81) P0_1;
__sbit __at (0x82) P0_2;
__sbit __at (0x83) P0_3;
__sbit __at (0x84) P0_4;
__sbit __at (0x85) P0_5;
__sbit __at (0x86) P0_6;
__sbit __at (0x87) P0_7;

__sfr __at (0x94) P0DIR;
__sfr __at (0x95) P0ALT;
__sfr __at (0xC9) P0EXP;

//SPI slave
__sfr __at (0xBC) SSCONF;
__sfr __at (0xBD) SSDAT;
__sfr __at (0xBE) SSSTAT;

//Interrupt basics
__sfr __at(0xA8) IEN0;
__sfr __at(0xB8) IEN1;
__sfr __at(0xA9) IP0;
__sfr __at(0xB9) IP1;

__sfr  __at (0xC0) IRCON;
__sbit __at (0xC0) RFRDY;
__sbit __at (0xC1) RFIRQ;
__sbit __at (0xC2) SDONE;  //Master or slave SPI done
__sbit __at (0xC3) USBWU;
__sbit __at (0xC4) USBIRQ_F;
__sbit __at (0xC5) WU;
__sbit __at (0xC6) TF2;
__sbit __at (0xC7) EXF2;

//Timer registers
__sfr  __at (0x88) TCON;
__sfr  __at (0x89) TMOD;
__sfr  __at (0x8A) TL0;
__sfr  __at (0x8B) TL1;
__sfr  __at (0x8C) TH0;
__sfr  __at (0x8D) TH1;
__sfr  __at (0xC8) T2CON;
__sfr  __at (0xCC) TL2;
__sfr  __at (0xCD) TH2;

__sfr  __at (0xC1) CCEN;
__sfr  __at (0xC2) CCL1;
__sfr  __at (0xC3) CCH1;
__sfr  __at (0xC4) CCL2;
__sfr  __at (0xC5) CCH2;
__sfr  __at (0xC6) CCL3;
__sfr  __at (0xC7) CCH3;
__sfr  __at (0xCA) CRCL;
__sfr  __at (0xCB) CRCH;

//Timer register values
// TCON
#define TCON_IT0 (0x01)
#define TCON_IE0 (0x02)
#define TCON_IT1 (0x04)
#define TCON_IE1 (0x08)
#define TCON_TR0 (0x10)
#define TCON_TF0 (0x20)
#define TCON_TR1 (0x40)
#define TCON_TF1 (0x80)

// TMOD
#define TMOD_T0_MODE_MASK (0x03)
#define TMOD_T0_MODE0 (0x00)
#define TMOD_T0_MODE1 (0x01)
#define TMOD_T0_MODE2 (0x02)
#define TMOD_T0_MODE3 (0x03)
#define TMOD_CT0 (0x04)
#define TMOD_GATE0 (0x08)
#define TMOD_T1_MODE_MASK (0x30)
#define TMOD_T1_MODE0 (0x00)
#define TMOD_T1_MODE1 (0x10)
#define TMOD_T1_MODE2 (0x20)
#define TMOD_T1_MODE3 (0x30)
#define TMOD_CT1 (0x40)

// T2CON
#define T2CON_T2I_MASK     (0x03)
#define T2CON_STOPPED      (0x00)
#define T2CON_T2I_INTERNAL (0x01)
#define T2CON_T2I_EXTERNAL (0x02)
#define T2CON_T2I_GATED    (0x03)
#define T2CON_T2CM         (0x04)
#define T2CON_T2R_MASK     (0x18)
#define T2CON_T2R_DISABLE  (0x00)
#define T2CON_T2R_MODE0    (0x10)
#define T2CON_T2R_MODE1    (0x18)
#define T2CON_I2FR         (0x20)
#define T2CON_I3FR         (0x40)
#define T2CON_T2PS_12      (0x00)
#define T2CON_T2PS_24      (0x01)

// CCEN
#define CCEN_COCA0_MASK    (0x03)
#define CCEN_COCA0_DISABLE (0x00)
#define CCEN_COCA0_COMPARE (0x02)
#define CCEN_COCA0_CAPTURE (0x03)
#define CCEN_COCA1_MASK    (0x0C)
#define CCEN_COCA1_DISABLE (0x00)
#define CCEN_COCA1_CAPTURE (0x0C)
#define CCEN_COCA2_MASK    (0x30)
#define CCEN_COCA2_DISABLE (0x00)
#define CCEN_COCA2_CAPTURE (0x30)
#define CCEN_COCA3_MASK    (0xC0)
#define CCEN_COCA3_DISABLE (0x00)
#define CCEN_COCA3_CAPTURE (0xC0)

//FLASH registers
__sfr __at (0xF8) FSR;
__sfr __at (0xF9) FPCR;
__sfr __at (0xFA) FCR;

//FLASH register values
//FSR
#define FSR_RDISIP (1<<1)
#define FSR_RDISMB (1<<2)
#define FSR_INFEN  (1<<3)
#define FSR_RDYN   (1<<4)
#define FSR_WEN    (1<<5)
#define FSR_STP    (1<<6)
#define FSR_DBG    (1<<7)

//USB registers
//SFR part
__sfr  __at (0xA0) USBCON;
__sbit __at (0xA0) USB_IV0;
__sbit __at (0xA1) USB_IV1;
__sbit __at (0xA2) USB_IV2;
__sbit __at (0xA3) USB_IV3;
__sbit __at (0xA4) USB_IV4;
__sbit __at (0xA5) USB_SUSPEND;
__sbit __at (0xA6) USB_WU;
__sbit __at (0xA7) USB_SWRDT;

__sfr __at (0xD9) USBSLP;

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

unsigned char __at (0xC7E8) SETUPBUF[8];

//EP data buffers
unsigned char __at (0xC6C0) OUT0BUF[64];
unsigned char __at (0xC700) IN0BUF[64];
unsigned char __at (0xC640) OUT1BUF[64];
unsigned char __at (0xC680) IN1BUF[64];
unsigned char __at (0xC5C0) OUT2BUF[64];
unsigned char __at (0xC600) IN2BUF[64];
unsigned char __at (0xC540) OUT3BUF[64];
unsigned char __at (0xC580) IN3BUF[64];
unsigned char __at (0xC4C0) OUT4BUF[64];
unsigned char __at (0xC500) IN4BUF[64];
unsigned char __at (0xC440) OUT5BUF[64];
unsigned char __at (0xC480) IN5BUF[64];


/******* USB Registers content ********/
//USBCON
#define SUSPEND 0x20
#define WU 0x40
#define SWRSR 0x80

//USBSLP
#define SLEEP 0x01

//USBCS
#define SIGRSUME 0x01
#define FORCEJ 0x02
#define DISCON 0x04
#define SOFGEN 0x20
#define WAKESRC 0x80

//USB interrupt, content of IVEC
#define IRQ_SUDAV (0x00)
#define IRQ_SOF (0x04)
#define IRQ_SUTOK (0x08)
#define IRQ_SUSPEND (0x0C)
#define IRQ_USBRESET (0x10)
#define IRQ_EP0IN (0x18)
#define IRQ_EP0OUT (0x1C)
#define IRQ_EP1IN (0x20)
#define IRQ_EP1OUT (0x24)
#define IRQ_EP2IN (0x28)
#define IRQ_EP2OUT (0x2C)
#define IRQ_EP3IN (0x30)
#define IRQ_EP3OUT (0x34)
#define IRQ_EP4IN (0x38)
#define IRQ_EP4OUT (0x3C)
#define IRQ_EP5IN (0x40)
#define IRQ_EP5OUT (0x44)

//EP0CS
#define EP0STALL 0x01
#define HSNAK    0x02
#define INBSY    0x04
#define OUTBSY   0x08
#define DSTALL   0x10
#define CHGSET   0x20

//IN/OUTxCS
#define EPSTALL 0x01
#define EPBSY  0x02

//USBIEN
#define SUDAVIE 0x01
#define SOFIE   0x02
#define SUTOKIE 0x04
#define SUSPIE  0x08
#define URESIE  0x10

//USBIRQ
#define SUDAVIR 0x01
#define SOFIR   0x02
#define SUTOKIR 0x04
#define SUSPIR  0x08
#define URESIR  0x10

//INBULKVAL
#define IN0VAL 0x01
#define IN1VAL 0x02
#define IN2VAL 0x04
#define IN3VAL 0x08
#define IN4VAL 0x10
#define IN5VAL 0x20


//IN_IEN
#define IN0IE 0x01
#define IN1IE 0x02
#define IN2IE 0x04
#define IN3IE 0x08
#define IN4IE 0x10
#define IN5IE 0x20

//IN_IRQ
#define IN0IR 0x01
#define IN1IR 0x02
#define IN2IR 0x04
#define IN3IR 0x08
#define IN4IR 0x10
#define IN5IR 0x20

//OUTBULKVAL
#define OUT0VAL 0x01
#define OUT1VAL 0x02
#define OUT2VAL 0x04
#define OUT3VAL 0x08
#define OUT4VAL 0x10
#define OUT5VAL 0x20

//OUT_IEN
#define OUT0IE 0x01
#define OUT1IE 0x02
#define OUT2IE 0x04
#define OUT3IE 0x08
#define OUT4IE 0x10
#define OUT5IE 0x20

//OUT_IRQ
#define OUT0IR 0x01
#define OUT1IR 0x02
#define OUT2IR 0x04
#define OUT3IR 0x08
#define OUT4IR 0x10
#define OUT5IR 0x20

//BC
#define BCDUMMY 1



#endif /* __NRF24LU1P_H__ */

