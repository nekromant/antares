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
#include "mcortex-hw.h"


void mctx_init()
{
	uint16_t* pscale = FPGA_MEM+12;
	*pscale = 0x5; //12000, out clock is 1Khz
	uint16_t* top =  FPGA_MEM+16;
	*top = 19944; //For servos
	top =  FPGA_MEM+14;
	*top=1000;
}

#define MCTX_INFOREG	(char*) (FPGA_MEM+512)

void mctx_dump_inforeg()
{
	char* m = MCTX_INFOREG;
	printk("mctx: Motor cortex FPGA driver :: IP Core ID 0x%hhx\n\r", *m);
	printk("mctx: (c) Necromant 2011-2012\n\r");
	printk("mctx: Dumping avaliable hardware\n\r");
	m++;
	printk("mctx: %hhd Servo OCs avaliable\n\r",*m);
	m++;
	printk("mctx: %hhd Motor OCs avaliable\n\r",*m);
	m++;
	printk("mctx: %hhd Quadrature Encoder Counters with %hhd bits filtering avaliable\n\r",*m, *(MCTX_INFOREG+6));
	m++;
	printk("mctx: %hhd RGB Sensor Channels (Round-Robin) avaliable\r\n",*m);
	m++;
	printk("mctx: %hhd Ultrasonic Ranging Channels (Round-Robin) avaliable\r\n",*m);
	
}

void mctx_servo_test()
{
	int s;
	uint16_t n=0;
	while (1)
	{
	printk("servo: ");
	scanf("%d", &s);
	scanf("%hd", &n);
	printk("Angle: %hd/19944\n\r",n);
 	sleep_ticks(50);
	mctx_set_servo_pwm(s,n);
	}
}

//PA8 PC11

/* reset encoder counters */
void mctx_enc_reset(int enc)
{
  
	// should work
        *MCTX_ENC_CTL=0xff;
	mdelay(100);
        *MCTX_ENC_CTL=0x0;
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
*MCTX_MOTOR_CTL&=~(3<<(motor*2));
*MCTX_MOTOR_CTL|=(dir<<(motor*2));
//printk("mt %d %d\n\r",motor, dir);
}

void mctx_set_servo_pwm(int servo, short value)
{
	uint16_t* pwm = MCTX_SPWM + servo;
	*pwm = value;
}


void mctx_set_motor_pwm(int motor, short value)
{
	uint16_t* pwm = MCTX_MPWM + motor;
	*pwm = value;
}

void mctx_reset_dir()
{
	
	mctx_set_motor_pwm(0,600);
	mctx_set_motor_pwm(1,600);
	mctx_set_motor_dir(0, MCTX_MBCK);
	mctx_set_motor_dir(1, MCTX_MBCK);
	while ((mctx_endtoggle(0)) || (mctx_endtoggle(1)))
	{
		asm("nop");
	}
	mctx_set_motor_pwm(0,0);
	mctx_set_motor_pwm(1,0);
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