//#define STM32F10X_LD
#include <arch/antares.h>
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "core_cm3.h"

#include <lib/tasks.h>

void SysTick_Handler()
{
    //GPIO_Write(GPIOA, 0x0000);
    task_manager();
}

void turn_on(void);
void turn_off(void);

static  GPIO_InitTypeDef gpio_leds = {
	.GPIO_Speed = GPIO_Speed_50MHz,
	.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6,
	.GPIO_Mode = GPIO_Mode_Out_PP,
};

handler_t task_TurnOn = {
    .handler = &turn_on,
    .uptime = 10
};

handler_t task_TurnOff = {
    .handler = &turn_off,
    .uptime = 110
};

int main()
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		GPIO_Init(GPIOA, &gpio_leds);
        //GPIO_Write(GPIOA, 0xFFFF);
		// Init SysTick
        SysTick_Config(SystemCoreClock / 100);

        add_handler(&task_TurnOn);
        add_handler(&task_TurnOff);

        while(1);;;
}

void turn_on(void)
{
    GPIO_Write(GPIOA, 0xFFFF);
    task_TurnOn.uptime += 200;
    add_handler(&task_TurnOn);
}

void turn_off(void)
{
    GPIO_Write(GPIOA, 0x0000);
    task_TurnOff.uptime += 200;
    add_handler(&task_TurnOff);
}
