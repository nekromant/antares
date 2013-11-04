#include <lib/earlycon.h>
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"


/* TODO: Move to menuconfig */

#define CONFIG_LIB_EARLYCON_STM32F1X_UART 1

#if CONFIG_LIB_EARLYCON_STM32F1X_UART == 1
#define USART_PERIPH RCC_APB2Periph_USART1
#endif

#if CONFIG_LIB_EARLYCON_STM32F1X_UART == 2
#define USART_PERIPH RCC_APB1Periph_USART2
#endif

#if CONFIG_LIB_EARLYCON_STM32F1X_UART == 3
#define USART_PERIPH RCC_APB1Periph_USART3
#endif

#if CONFIG_LIB_EARLYCON_STM32F1X_UART == 4
#define USART_PERIPH RCC_APB1Periph_UART4
#endif

#if CONFIG_LIB_EARLYCON_STM32F1X_UART == 5
#define USART_PERIPH RCC_APB1Periph_UART5
#endif



/* 
 * TODO: Fill in all parameters from menuconfig. 
 * webconn, this is the task for you!
 *
 */



void stm32f1x_uart_init() {
	GPIO_InitTypeDef    GPIO_InitStruct;
	USART_InitTypeDef    USART_InitStruct;
	USART_ClockInitTypeDef USART_ClockInitStructure;

	RCC_APB2PeriphClockCmd((RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA |  RCC_APB2Periph_AFIO), ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9; 
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStruct); 


	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOA, &GPIO_InitStruct); 
	
	
        USART_ClockStructInit(&USART_ClockInitStructure);
        USART_ClockInit(USART1, &USART_ClockInitStructure);

	USART_InitStruct.USART_BaudRate = 115200; 
	USART_InitStruct.USART_WordLength = USART_WordLength_8b; 
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No ;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStruct);

	USART_Cmd(USART1, ENABLE); 
}


static void stm32f1x_uart_putchar(char c)
{
	while(!(USART1->SR & USART_SR_TC)); 
	USART1->DR = c; 
}

static int stm32f1x_uart_getchar() {
	while (!(USART1->SR & USART_SR_RXNE));
	return (int) USART1->DR;
}


static int stm32f1x_uart_avail() {
	return (USART1->SR & USART_SR_RXNE);
}


struct early_console g_early_console = {
	.rxchar = stm32f1x_uart_getchar,
	.txchar = stm32f1x_uart_putchar,
	.init = stm32f1x_uart_init,		
	.havechar = stm32f1x_uart_avail,
};
