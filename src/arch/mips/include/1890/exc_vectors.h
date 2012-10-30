#ifndef _EXC_VECTORS_H_
#define _EXC_VECTORS_H_

/* ------------------------------------------------------- */
/* Exceptions & Interrupts vectors table */

#define VECT_BASE	0xa0000400

#define Int_v		0
#define TLBmod_v	8
#define TLBL_v		0x10
#define TLBS_v		0x18
#define ADEL_v		0x20
#define ADES_v		0x28
#define DBE_v		0x30
#define IBE_v		0x38
#define Sys_v		0x40
#define Bp_v		0x48
#define RI_v		0x50
#define	CpU_v		0x58
#define Ov_v		0x60
#define Reserved_exc_v	0x68
#define Tr_v		0x70
#define FPE_v		0x78

#define SysCtr_Int3_v	0x80
#define SysCtr_Int2_v	0x88
#define CP1_v		0x90
#define SysCtr_Int1_v	0x98
#define SysCtr_Int0_v	0xa0
#define Watchdog_v	0xa8
#define Soft_Int1_v	0xb0
#define Soft_Int0_v	0xb8

#define Reserved_int_v	0xc0
#define	RS_PortB_v	0xc8
#define	RS_PortA_v	0xd0
#define	ExtInt4_v	0xd8
#define	ExtInt3_v	0xe0
#define	ExtInt2_v	0xe8
#define	ExtInt1_v	0xf0
#define	WrErr_v		0xf8
#define	TimerC_v	0x100
#define	TimerB_v	0x108
#define	Pci_v		0x110

#define	DRAM_ECC_v	0x118
#define	PerCntr_v	0x120
#define	TimerA_v	0x128
#define	ExtInt0_v	0x130
#define	ParError_v	0x138
#define	IWE_v		0x140
#define	DWE_v		0x148
#define	CmpReg_v	0x150
#define	Int9_GT_v	0x158
#define Int8_AddFunc_v	0x160
#define Int7_PMC0_v	0x168
#define Int6_MultyIO_v	0x170
#define Int5_Eth_v	0x178
#define Int4_PMC0_v	0x180
#define Int3_PMC1_v	0x188
#define Int2_PMC1_v	0x190
#define Int1_PMC1_VME_v	0x198
#define Int0_PMC1_v	0x1a0
#define	RS_PortC_v	0x1a8
#define	TMC_v		0x1b0
#define	DBG_Exc_v	0x1b8


#define NVECT		0x1c0 /* Last vector. Must be changed on vector adding*/

#endif /* _EXC_VECTORS.H_ */
