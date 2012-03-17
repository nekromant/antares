//#define STM32F10X_LD
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

void TIM6_IRQHandler()
{
	
}


void delay(void)
{
        int i = 10000000;													/* About 1/4 second delay */
        while (i-- > 0) {
                asm("nop");													/* This stops it optimising code out */
        }
}


static  GPIO_InitTypeDef gpio_leds = {
	.GPIO_Speed = GPIO_Speed_50MHz,
	.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6,
	.GPIO_Mode = GPIO_Mode_Out_PP,
};

// static int a = 5;
// static int b;

// 
int main(void)
{
// 		int c;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		GPIO_Init(GPIOA, &gpio_leds);
 		GPIO_Write(GPIOA,0xffff);
        while(1);;;

}


