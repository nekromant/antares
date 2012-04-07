#include <arch/antares.h>
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "xsscu.h"

/*
JRST CCLK PB4
JTDO DIN PB3
JTCK PROGB PA14

JTDI INITB PA15
JTMS DONE PA13
*/


static void clk_ctl(char m)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_4, m ? 1 : 0);
}

static void progb_ctl(char m)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_14, m ? 1 : 0);
}

static void sout_ctl(char m)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_3, m ? 1 : 0);
}

static char get_done()
{
	return (char) GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13);
}

static char get_initb()
{
	return (char) GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15);
}

static void mdelay(int n)
{
        int i = n*100;													/* About 1/4 second delay */
        while (i-- > 0) {
                asm("nop");													/* This stops it optimising code out */
        }
}

static const struct xsscu_unit fpga0 = {
.name = "Xilinx XS3S100E",
.clk_ctl = clk_ctl,
.sout_ctl = sout_ctl,
.progb_ctl = progb_ctl,
.get_initb = get_initb,
.get_done = get_done,
.delay = mdelay,
.delay_pb = 1200,
.delay_clk = 0
};

static const GPIO_InitTypeDef xsscu_pai = {
.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15,
.GPIO_Mode = GPIO_Mode_IPU,
};

static const GPIO_InitTypeDef xsscu_pao = {
.GPIO_Speed = GPIO_Speed_50MHz,
.GPIO_Pin = GPIO_Pin_14,
.GPIO_Mode = GPIO_Mode_Out_PP,
};

static const GPIO_InitTypeDef xsscu_pbo = {
.GPIO_Speed = GPIO_Speed_50MHz,
.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4,
.GPIO_Mode = GPIO_Mode_Out_PP,
};

static  const GPIO_InitTypeDef fpga_clk = {
	.GPIO_Speed = GPIO_Speed_50MHz,
	.GPIO_Pin = GPIO_Pin_8,
	.GPIO_Mode = GPIO_Mode_Out_PP,
};

void fpga_enable_clock()
{
	GPIO_SetBits(GPIOG,GPIO_Pin_8);
}

void fpga_disable_clock()
{
	GPIO_ResetBits(GPIOG,GPIO_Pin_8);
}


void mcortex_fpga_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	GPIO_Init(GPIOA, (GPIO_InitTypeDef*) &xsscu_pao);
	GPIO_Init(GPIOA, (GPIO_InitTypeDef*) &xsscu_pai);
 	GPIO_Init(GPIOB, (GPIO_InitTypeDef*) &xsscu_pbo);
	GPIO_Init(GPIOG, (GPIO_InitTypeDef*) &fpga_clk);
	printk("fpga: Initialising %s\n\r",fpga0.name);
	
}

#define MAGIC	0xf0f0f0
struct fpga_hdr
{
	uint32_t magic;
	uint32_t len;
};

void mcortex_fpga_update()
{
	char buf[256];
	printk("fpga: erasing SPI flash...\n\r");
	sFLASH_EraseBulk();
	printk("fpga: Will now enter binary mode for flash upload\n\r");
	char m=0;
	do {
		m = getchar();
	}
	while (m!=0xF0);
}


#if 0
#include "fw.h"
void mcortex_fpga_fromflash()
{
	fpga_disable_clock();
	xsscu_reset(&fpga0);
	xsscu_write(&fpga0, fw, fw_size);
	xsscu_finalize(&fpga0, 1200);
	fpga_enable_clock();
}
#endif