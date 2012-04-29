//#define STM32F10X_LD
#include <arch/antares.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32_eval_spi_flash.h"
#include "stm32f10x_exti.h"
#include "misc.h"

#include <lib/tasks.h>
#include "mcortex-hw.h"

#include "hm_chassis.h"

void mdelay(int n)
{
        int i = n*10;                                                                                                   /* About 1/4 second delay */
        while (i-- > 0) {
                asm("nop");                                                                                                     /* This stops it optimising code out */
        }
}

void SysTick_Handler(void)
{
	tmgr_tick();
}

void Usart1Init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
USART_ClockInitTypeDef USART_ClockInitStructure;
//enable bus clocks
RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
//Set USART1 Tx (PA.09) as AF push-pull
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure);
 
//Set USART1 Rx (PA.10) as input floating
 
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 
GPIO_Init(GPIOA, &GPIO_InitStructure);
 
USART_ClockStructInit(&USART_ClockInitStructure);
 
USART_ClockInit(USART1, &USART_ClockInitStructure);
 
USART_InitStructure.USART_BaudRate = 115200;
 
USART_InitStructure.USART_WordLength = USART_WordLength_8b;
 
USART_InitStructure.USART_StopBits = USART_StopBits_1;
 
USART_InitStructure.USART_Parity = USART_Parity_No ;
 
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
 
//Write USART1 parameters
 
USART_Init(USART1, &USART_InitStructure);
 
//Enable USART1
 
USART_Cmd(USART1, ENABLE);
 
}

static GPIO_InitTypeDef motors_dir = {
  .GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15,
  .GPIO_Speed = GPIO_Speed_50MHz,
  .GPIO_Mode = GPIO_Mode_Out_PP
};

static GPIO_InitTypeDef motors_pwm = {
  .GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7,
  .GPIO_Speed = GPIO_Speed_50MHz,
  .GPIO_Mode = GPIO_Mode_AF_PP
};

static GPIO_InitTypeDef encoders_A = {
	.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14,
	.GPIO_Speed = GPIO_Speed_50MHz,
	.GPIO_Mode = GPIO_Mode_IN_FLOATING
};

static GPIO_InitTypeDef encoders_B = {
	.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10,
	.GPIO_Speed = GPIO_Speed_50MHz,
	.GPIO_Mode = GPIO_Mode_IN_FLOATING
};

static TIM_TimeBaseInitTypeDef pwm_timer = {
  .TIM_Prescaler = 1,
  .TIM_CounterMode = TIM_CounterMode_CenterAligned2,
  .TIM_Period = 65535,
  .TIM_ClockDivision = 0,
};

static TIM_OCInitTypeDef pwm_outs = {
  .TIM_OCMode = TIM_OCMode_PWM1,
  .TIM_OutputState = TIM_OutputState_Enable,
  .TIM_OCPolarity = TIM_OCPolarity_High,
  .TIM_Pulse = 0
};

inline void init_gpio(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

  GPIO_Init(GPIOB, &motors_dir);
  GPIO_Init(GPIOC, &motors_pwm);
  
  //GPIO_Init(GPIOA, &encoders_A);
  //GPIO_Init(GPIOC, &encoders_B);
}

inline void init_timer3(void)
{
  // Set clock to AFIO
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  // Remap TIM3 to GPIOC
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

  // Set clock to timer
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  // Init timer
  TIM_TimeBaseInit(TIM3, &pwm_timer);
  // Init outputs
  TIM_OC1Init(TIM3, &pwm_outs);
  TIM_OC2Init(TIM3, &pwm_outs);

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  // Run timer
  TIM_Cmd(TIM3, ENABLE);
}
#if 0
void int_cfg(GPIO_TypeDef * GPIO, uint16_t pin, uint32_t exti, uint16_t exti2)
{
  /* Enable GPIOA clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  
  /* Configure PA.00 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = 1<<pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIO, &GPIO_InitStructure);

  /* Enable AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  /* Connect EXTI0 Line to PA.00 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, pin);

  /* Configure EXTI0 line */
  EXTI_InitStructure.EXTI_Line = exti;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI0 Interrupt to the lowest priority */
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = exti2;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
#endif

#if 0
inline void init_inerrupts(void)
{
  /* Enable AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  /* Connect EXTI0 Line to PA.00 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource13 | GPIO_PinSource14);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8 | GPIO_PinSource10);

  /* Configure EXTIs lines */
  EXTI_InitTypeDef e13 = {
	  .EXTI_Line = EXTI_Line13,
	  .EXTI_Mode = EXTI_Mode_Interrupt,
	  .EXTI_Trigger = EXTI_Trigger_Rising,
	  .EXTI_LineCmd = ENABLE
  };
  
  EXTI_Init(&e13);
  EXTI_Init(&e14);
  EXTI_Init(&e10);
  EXTI_Init(&e8);

  /* Enable and set EXTI Interrupts to the lowest priority */
  
  NVIC_InitTypeDef n1 = {
	  .NVIC_IRQChannel
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
#endif

int main(void)
{
  //printk("Motor Cortex rev. 0.9 :: Antares uC system, v. %s\n\r", CONFIG_VERSION_STRING);
  SysTick_Config(SystemCoreClock / 1000);
  
//   int_cfg(GPIOA, 13, EXTI_Line13, EXTI0_IRQn);
//   int_cfg(GPIOA, 14, EXTI_Line14, EXTI0_IRQn);

  init_gpio();
  init_timer3();
  //init_adc();
  //Usart1Init();
  
  //setvbuf(stdin, NULL, _IONBF, 0);
  //setvbuf(stdout, NULL, _IONBF, 0);
  //setvbuf(stderr, NULL, _IONBF, 0);

  //printf("\xFF\n\r\n\r");
  //printk("Hatul-Madan co-operating bot initialized\n\r");
  
  GPIO_InitTypeDef shmorg = {
	  .GPIO_Pin = GPIO_Pin_11,
	  .GPIO_Speed = GPIO_Speed_2MHz,
	  .GPIO_Mode = GPIO_Mode_IPD
  };
  GPIO_Init(GPIOB, &shmorg);
  
  while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11));
  
  
  chassis_write(forward, forward, 32767, 32767);
  sleep_ticks(1100);
  chassis_stop();
  sleep_ticks(1000);
  chassis_write(forward, backward, 17000, 17000);
  sleep_ticks(300);
  chassis_stop();
  sleep_ticks(1000);
  chassis_write(backward, backward, 17000, 17000);
  sleep_ticks(800);
  chassis_stop();
 
  while(1);;;

  return 0;
}
