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
#include "mcortex-hw.h"


//PB6

struct mctx_btn {
	GPIO_TypeDef * gpio;
	uint16_t pin;
};

struct mctx_btn odc[] = 
{
	{ GPIOE, GPIO_Pin_2 },
	{ GPIOE, GPIO_Pin_4 },
	{ GPIOE, GPIO_Pin_6 },
	{ GPIOC, GPIO_Pin_14 }
};

struct mctx_btn endtoggle[] = 
{
	{ GPIOA, GPIO_Pin_8 },
	{ GPIOC, GPIO_Pin_11 },
};




static GPIO_InitTypeDef pins = 
{
	.GPIO_Speed = GPIO_Speed_50MHz,
	.GPIO_Mode = GPIO_Mode_IPU,
};

void mcortex_btns_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	pins.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_14;
	GPIO_Init(GPIOC, &pins);
	pins.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &pins);
	pins.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_6 ;
	GPIO_Init(GPIOE, &pins);
	pins.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOB, &pins);
	
}


uint8_t mctx_odct(int side)
{
	return GPIO_ReadInputDataBit(odc[side].gpio, odc[side].pin);
}

uint8_t mctx_endtoggle(int n)
{
	return GPIO_ReadInputDataBit(endtoggle[n].gpio, endtoggle[n].pin);
}


uint8_t mctx_scond_read()
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);
}




void mctx_etest()
{
	while (1)
	{
	printk("endtoggle = { %hhx, %hhx }; odct = { %hhx, %hhx, %hhx, %hhx };\r", 
	mctx_endtoggle(0),
	mctx_endtoggle(1),
	mctx_odct(0),
	mctx_odct(1),
	mctx_odct(2),
	mctx_odct(3),
	mctx_scond_read()
	);
	}
}


