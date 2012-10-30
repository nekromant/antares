#ifndef _KERNEL_H_
#define _KERNEL_H_

#ifdef _ASMLANGUAGE

/* Stack base for kernel */
#define KERNEL_STACK	0xa0002000
#define EXCEPTION_STACK	0xa0001000

/* System calls */
#define MonCall_LO	0x110

	.macro	MonCall m1 
	lui	r31,0xbfc0
	ori	r31,r31,MonCall_LO+\m1
	jalr	r31,r31
	nop
	.endm

	.macro	NEXT
	lw	r20,(r16)
	addiu	r16,r16,4
	addu	r20,r20,r22
	jr	r20
	nop
	.endm

#define ExcConnect	0x0
#define	Tob		0x8

/* MAX CALL NUMBER = 0xd*8=0x68!!! */

#endif /* _ASMLANGUAGE */

#define PRID_R3081	0x00000230
#define PRID_1B578	0x00000230
#define PRID_1B812	0x01000230
#define PRID_KOMDIV	0x01000231



/* Variables & save area */
#define SREGS		0xa0000200

#define SR0 0
#define SR1 0x4
#define SR2 0x8
#define SR3 0xc
#define SR4 0x10
#define SR5 0x14
#define SR6 0x18
#define SR7 0x1c
#define SR8 0x20
#define SR9 0x24
#define SR10 0x28
#define SR11 0x2c
#define SR12 0x30
#define SR13 0x34
#define SR14 0x38
#define SR15 0x3c
#define SR16 0x40
#define SR17 0x44
#define SR18 0x48
#define SR19 0x4c
#define SR20 0x50
#define SR21 0x54
#define SR22 0x58
#define SR23 0x5c
#define SR24 0x60
#define SR25 0x64
#define SR26 0x68
#define SR27 0x6c
#define SR28 0x70
#define SR29 0x74
#define SR30 0x78
#define SR31 0x7c

#define SEPC	0x80
#define SSR	0x84
#define SBADVADDR 0x88
#define SCAUSE	0x8c
#define SCONTEXT 0x90

#define EXCTYPE	  0x94
#define INTFLAG	  0x98 
#define SINDEX    0x9c
#define STLBLO    0xa0
#define STLBHI    0xa4
/* #define BFDATADR  0xa8 */
#define AGOAC     0xac  /* procedure GOAC address */
#define AGOA      0xb0  /* procedure GOA address */
#define INTVECT   0xb4
#define TFKNFLAG  0xb8  /* Flag: 0 - TFKN, 1 - TMS, 2 - krah TFKN */
#define GOACFLAG  0xbc  /* 1 - Goac (cont), 0 - Goa (go) */
/* #define INTTESTFLAG 0xc0 */  /* Flag: 1 - interrupt test, 0 - not */
/* #define BUSERRDBE 0xc4 */

#define SHI	0xc8
#define SLO	0xcc

#define SCONFIG   0xd0

/* #define WRERRFLAG 0xd8 */
/* #define BEV	  0xdc */
/* #define TLB_INDEX 0xe0 */

#define EXCFLAG	  0xe4

#define SWITCHSTATE  0xe8

#define SFC0	0xec
#define SFC31	0xf0

#define TLBIMAGE  0x100 /* 0x300 - 0x500 : TLB */

#define SFPR0	0x300	/* 0xa0000500 */
#define SFPR1	0x304
#define SFPR2	0x308
#define SFPR3	0x30c
#define SFPR4	0x310
#define SFPR5	0x314
#define SFPR6	0x318
#define SFPR7	0x31c
#define SFPR8	0x320
#define SFPR9	0x324
#define SFPR10	0x328
#define SFPR11	0x32c
#define SFPR12	0x330
#define SFPR13	0x334
#define SFPR14	0x338
#define SFPR15	0x33c
#define SFPR16	0x340
#define SFPR17	0x344
#define SFPR18	0x348
#define SFPR19	0x34c
#define SFPR20	0x350
#define SFPR21	0x354
#define SFPR22	0x358
#define SFPR23	0x35c
#define SFPR24	0x360
#define SFPR25	0x364
#define SFPR26	0x368
#define SFPR27	0x36c
#define SFPR28	0x370
#define SFPR29	0x374
#define SFPR30	0x378
#define SFPR31	0x37c
	/* 0xa0000580 */
/* restart PRIME */
#define LONGJMP_BUF_PTR 0x380

#endif /* _KERNEL.H_ */
