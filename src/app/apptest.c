//#define STM32F10X_LD
#include <arch/antares.h>
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
	.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_8,
	.GPIO_Mode = GPIO_Mode_Out_PP,
};




uint16_t v;

////////////////////// ENCODERS
static uint32_t val_a_a, val_a_b, val_b_a, val_b_b;
void encoders_reset()
{
  val_a_a=0;
  val_a_b=0;
  val_b_a=0;
  val_b_b=0;
}
void enc_a_a()
{
  val_a_a++;
}
void enc_a_b()
{
  val_a_b++;
}
void enc_b_a()
{
  val_b_a++;
}
void enc_b_b()
{
  val_b_b++;
}



void timer_enable()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	TIM_TimeBaseInit(TIM1,&tim);
}


//M1A PB14 33
//M1B PB15 34
//M1E PC7 36 PWM
//M2A PB13 32
//M2B PB12 31
//M2E PC6 35 PWM


static TIM_TimeBaseInitTypeDef tim = 
{
	.TIM_Prescaler=240,
	.TIM_CounterMode=TIM_CounterMode_Up,
	.TIM_Period=1000,
	.TIM_ClockDivision = TIM_CKD_DIV4   
};
	
TIM_OCInitTypeDef oc = 
{
	.TIM_OutputState = TIM_OutputState_Enable,
	.TIM_OCMode = TIM_OCMode_PWM1,
	.TIM_Pulse = 0
};

GPIO_InitTypeDef mpwm = 
{
	.GPIO_Speed = GPIO_Speed_50MHz,
	.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7,
	.GPIO_Mode = GPIO_Mode_Out_AF_PP
};

int main()
{
		encoders_reset();
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		GPIO_Init(GPIOA, &gpio_leds);
		GPIO_Write(GPIOA,0xffff);
		GPIO_Init(GPIOC, &mpwm);
		
// 		GPIO_Write(GPIOA,0xffff);
		
		
        while(1);;;
}


