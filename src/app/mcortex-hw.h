#define FPGA_MEM ((uint32_t)0x68000000)

#define MCTX_LED_CTL	(char*) (FPGA_MEM+0)
#define MCTX_MOTOR_CTL	(char*) (FPGA_MEM+1)
#define MCTX_ENC_CTL	(char*) (FPGA_MEM+2)
#define MCTX_ADV_CTL	(char*) (FPGA_MEM+3)
#define MCTX_ENC0V		(int*)  (FPGA_MEM+4)
#define MCTX_ENC1V		(int*)  (FPGA_MEM+8)
#define MCTX_PSCALE		(short*)  (FPGA_MEM+12)

#define MCTX_M_TOP		(short*)  (FPGA_MEM+14)
#define MCTX_S_TOP		(short*)  (FPGA_MEM+16)
#define MCTX_MPWM		(short*)  (FPGA_MEM+18)
#define MCTX_SPWM		(short*)  (FPGA_MEM+22)

#define MCTX_MSTOP	0
#define MCTX_MFWD	1
#define MCTX_MBCK	2
