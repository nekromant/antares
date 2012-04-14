#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_adc.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_rcc.h>

#include "hm_chassis.h"

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

static TIM_TimeBaseInitTypeDef pwm_timer = {
  .TIM_Prescaler = 16,
  .TIM_CounterMode = TIM_CounterMode_Up,
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
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  GPIO_Init(GPIOB, &motors_dir);
  GPIO_Init(GPIOC, &motors_pwm);
}

inline void init_timer(void)
{
  // Set clock to AFIO
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  // Remap TIM3 to GPIOC
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

  // Set clock to timer
  RCC_APB2PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
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

inline void init_adc(void)
{

}

int main(void)
{
  init_gpio();
  init_timer();
  init_adc();

  int i, c;

  for(i=0; i<65536; i+=127)
  {
    chassis_write(forward, forward, i, i);
    //printk("Trying PWM output with length %d\n\r", i);
    for(c=0; c<299999; c++)
    {
      asm("nop");
    }
  }
  chassis_stop();
  while(1);;;

  return 0;
}
