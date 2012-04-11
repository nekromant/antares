#include <arch/antares.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>
#include <lib/xmodem.h>
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_fsmc.h"
#include "stm32_eval_spi_flash.h"
#include "misc.h"
#include "xsscu.h"


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


void mctx_init()
{
	uint16_t* pscale = FPGA_MEM+12;
	*pscale = 0x5; //12000, out clock is 1Khz
	uint16_t* top =  FPGA_MEM+16;
	*top = 19944; //For servos
	top =  FPGA_MEM+14;
	*top=1000;
}

/* reset encoder counters */
void mctx_enc_reset(int enc)
{
	*MCTX_ENC_CTL|=(1<<enc);
	*MCTX_ENC_CTL&=~(1<<enc);
}

/* inversion of channels */
void  mctx_enc_invert(int enc, int inv)
{
	if (inv)
		*MCTX_ENC_CTL|=(1<<(4+enc));
		else
		*MCTX_ENC_CTL&=~(1<<(4+enc));
}

/* read of encoder bits*/
int mctx_enc_read(int enc)
{
	
	switch(enc)
	{
		case 0: return *MCTX_ENC0V;
		case 1: return *MCTX_ENC1V;
	}
	return 0;
}

void mctx_set_motor_dir(int motor, int dir)
{
	*MCTX_MOTOR_CTL&=~(3<<motor); 
	*MCTX_MOTOR_CTL|=(dir<<motor);
}

void mctx_set_servo_pwm(int servo, short value)
{
	uint16_t* pwm = MCTX_SPWM + servo*2;
	*pwm = value;
}

void mctx_set_motor_pwm(int motor, short value)
{
	uint16_t* pwm = MCTX_MPWM + motor*2;
	*pwm = value;
}


void mctx_enc_dumploop()
{
	while(1)
	{
		printf("\r encoders = { a = %d; b = %d }; ", mctx_enc_read(0), mctx_enc_read(1) );
		if (try_getc(1)>0) break;
		mdelay(100);
	}
}