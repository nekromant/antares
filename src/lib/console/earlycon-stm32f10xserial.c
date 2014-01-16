#include <lib/earlycon.h>
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"

/* Hardware configuration constants */

#ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_PORT_USART1
        #define USART_PERIPH RCC_APB2Periph_USART1
        #define USART_REG USART1
        
        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_REMAP_1)
                #define USART_TX_PIN GPIO_Pin_6
                #define USART_TX_GPIO GPIOB
                
                #define USART_RX_PIN GPIO_Pin_7
                #define USART_RX_GPIO GPIOB
                
                #define USART_CK_PIN GPIO_Pin_8
                #define USART_CK_GPIO GPIOA

                #define USART_CTS_PIN GPIO_Pin_11
                #define USART_CTS_GPIO GPIOA

                #define USART_RTS_PIN GPIO_Pin_12
                #define USART_RTS_GPIO GPIOA

                #define GPIO_PERIPH (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB)
        #else
                #define USART_TX_PIN GPIO_Pin_9
                #define USART_TX_GPIO GPIOA
                
                #define USART_RX_PIN GPIO_Pin_10
                #define USART_RX_GPIO GPIOA
                
                #define USART_CK_PIN GPIO_Pin_8
                #define USART_CK_GPIO GPIOA

                #define USART_CTS_PIN GPIO_Pin_11
                #define USART_CTS_GPIO GPIOA

                #define USART_RTS_PIN GPIO_Pin_12
                #define USART_RTS_GPIO GPIOA

                #define GPIO_PERIPH RCC_APB2Periph_GPIOA
        #endif
#endif

#ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_PORT_USART2
        #define USART_PERIPH RCC_APB1Periph_USART2
        #define USART_REG USART2

        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_REMAP_2)
                #define USART_TX_PIN GPIO_Pin_5
                #define USART_TX_GPIO GPIOD

                #define USART_RX_PIN GPIO_Pin_6
                #define USART_RX_GPIO GPIOD

                #define USART_CK_PIN GPIO_Pin_7
                #define USART_CK_GPIO GPIOD
                
                #define USART_CTS_PIN GPIO_Pin_3
                #define USART_CTS_GPIO GPIOD
                
                #define USART_RTS_PIN GPIO_Pin_4
                #define USART_RTS_GPIO GPIOD

                #define GPIO_PERIPH RCC_APB2Periph_GPIOD
        #else
                #define USART_TX_PIN GPIO_Pin_2
                #define USART_TX_GPIO GPIOA
                
                #define USART_RX_PIN GPIO_Pin_3
                #define USART_RX_GPIO GPIOA
                
                #define USART_CK_PIN GPIO_Pin_4
                #define USART_CK_GPIO GPIOA
                
                #define USART_CTS_PIN GPIO_Pin_0
                #define USART_CTS_GPIO GPIOA
                
                #define USART_RTS_PIN GPIO_Pin_1
                #define USART_RTS_GPIO GPIOA
                
                #define GPIO_PERIPH RCC_APB2Periph_GPIOA
        #endif
#endif

#ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_PORT_USART3
        #define USART_PERIPH RCC_APB1Periph_USART3
        #define USART_REG USART3

        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_REMAP_3_FULL)
                #define USART_TX_PIN GPIO_Pin_8
                #define USART_TX_GPIO GPIOD
                
                #define USART_RX_PIN GPIO_Pin_9
                #define USART_RX_GPIO GPIOD
                
                #define USART_CK_PIN GPIO_Pin_10
                #define USART_CK_GPIO GPIOD
                
                #define USART_CTS_PIN GPIO_Pin_11
                #define USART_CTS_GPIO GPIOD
                
                #define USART_RTS_PIN GPIO_Pin_12
                #define USART_RTS_GPIO GPIOD
                
                #define GPIO_PERIPH RCC_APB2Periph_GPIOD
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_REMAP_3_PART)
                #define USART_TX_PIN GPIO_Pin_10
                #define USART_TX_GPIO GPIOC
                
                #define USART_RX_PIN GPIO_Pin_11
                #define USART_RX_GPIO GPIOC
                
                #define USART_CK_PIN GPIO_Pin_12
                #define USART_CK_GPIO GPIOC
                
                #define USART_CTS_PIN GPIO_Pin_13
                #define USART_CTS_GPIO GPIOB
                
                #define USART_RTS_PIN GPIO_Pin_14
                #define USART_RTS_GPIO GPIOB
                
                #define GPIO_PERIPH (RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB)
        #else
                #define USART_TX_PIN GPIO_Pin_10
                #define USART_TX_GPIO GPIOB
                
                #define USART_RX_PIN GPIO_Pin_11
                #define USART_RX_GPIO GPIOB
                
                #define USART_CK_PIN GPIO_Pin_12
                #define USART_CK_GPIO GPIOB
                
                #define USART_CTS_PIN GPIO_Pin_13
                #define USART_CTS_GPIO GPIOB
                
                #define USART_RTS_PIN GPIO_Pin_14
                #define USART_RTS_GPIO GPIOB

                #define GPIO_PERIPH RCC_APB2Periph_GPIOB
        #endif
#endif

#ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_PORT_UART4
        #define USART_PERIPH RCC_APB1Periph_UART4
        #define USART_REG UART4

        #define USART_TX_PIN GPIO_Pin_10
        #define USART_TX_GPIO GPIOC

        #define USART_RX_PIN GPIO_Pin_11
        #define USART_RX_GPIO GPIOC
        
        #define GPIO_PERIPH RCC_APB2Periph_GPIOC
#endif

#ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_PORT_UART5
        #define USART_PERIPH RCC_APB1Periph_UART5
        #define USART_REG UART5
        
        #define USART_TX_PIN GPIO_Pin_12
        #define USART_TX_GPIO GPIOC

        #define USART_RX_PIN GPIO_Pin_2
        #define USART_RX_GPIO GPIOD
        
        #define GPIO_PERIPH (RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD)
#endif

#if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_PARITY_NONE)
        #define USART_PARITY USART_Parity_No
#elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_PARITY_EVEN)
        #define USART_PARITY USART_Parity_Even
#elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_PARITY_ODD)
        #define USART_PARITY USART_Parity_Odd
#endif

#if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_STOP_05)
        #define USART_STOP USART_StopBits_0_5
#elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_STOP_1)
        #define USART_STOP USART_StopBits_1
#elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_STOP_15)
        #define USART_STOP USART_StopBits_1_5
#elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_STOP_2)
        #define USART_STOP USART_StopBits_2
#endif

#if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_RX) && defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_TX)
        #define USART_MODE (USART_Mode_Rx | USART_Mode_Tx)
#elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_RX)
        #define USART_MODE USART_Mode_Rx
#elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_TX)
        #define USART_MODE USART_Mode_Tx
#endif

#if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_CTS) && defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_RTS)
        #define USART_HW USART_HardwareFlowControl_RTS_CTS
#elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_CTS)
        #define USART_HW USART_HardwareFlowControl_CTS
#elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_RTS)
        #define USART_HW USART_HardwareFlowControl_RTS
#endif

#if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_CLOCK_HIGH)
        #define USART_CLOCK_POL USART_CPOL_High;
#elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_CLOCK_LOW)
        #define USART_CLOCK_POL USART_CPOL_Low;
#endif

#if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_CLOCK_FIRST)
        #define USART_CLOCK_PHASE USART_CPHA_1Edge
#elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_CLOCK_SECOND)
        #define USART_CLOCK_PHASE USART_CPHA_2Edge
#endif

#if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_CLOCK_LASTBIT)
        #define USART_CLOCK_LAST USART_LastBit_Enable
#else
        #define USART_CLOCK_LAST USART_LastBit_Disable
#endif

#ifndef USART_HW
#define USART_HW USART_HardwareFlowControl_None
#endif


void stm32f10x_usart_init() {
	GPIO_InitTypeDef    GPIO_InitStruct;
	USART_InitTypeDef    USART_InitStruct;

        /* 0. Configure RCC */
        RCC_APB2PeriphClockCmd(GPIO_PERIPH | RCC_APB2Periph_AFIO, ENABLE);
        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_PORT_USART1)
        RCC_APB2PeriphClockCmd(USART_PERIPH, ENABLE);
        #else
        RCC_APB1PeriphClockCmd(USART_PERIPH, ENABLE);
        #endif
        
        /* 1. Configure pin remapping */
        #ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_PORT_USART1
                #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_REMAP_1)
                GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
                #else
                GPIO_PinRemapConfig(GPIO_Remap_USART1, DISABLE);
                #endif
        #endif

        #ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_PORT_USART2
                #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_REMAP_2)
                GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
                #else
                GPIO_PinRemapConfig(GPIO_Remap_USART2, DISABLE);
                #endif
        #endif

        #ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_PORT_USART3
                #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_REMAP_3_PART)
                GPIO_PinRemapConfig(GPIO_FullRemap_USART3, DISABLE);
                GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);
                #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_REMAP_3_FULL)
                GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, DISABLE);
                GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
                #else
                GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, DISABLE);
                GPIO_PinRemapConfig(GPIO_FullRemap_USART3, DISABLE);
                #endif
        #endif

        /* 2. Configure GPIO */
        #ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_TX
        /* TX configuration */
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStruct.GPIO_Pin = USART_TX_PIN;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(USART_TX_GPIO, &GPIO_InitStruct);
        #endif

        #ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_RX
        /* RX configuration */
        GPIO_InitStruct.GPIO_Pin = USART_RX_PIN;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(USART_RX_GPIO, &GPIO_InitStruct);
        #endif

        #ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_SYNC
        /* CK configuration */
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStruct.GPIO_Pin = USART_CK_PIN;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(USART_CK_GPIO, &GPIO_InitStruct);
        #endif

        #ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_CTS
        /* CTS configuration */
        GPIO_InitStruct.GPIO_Pin = USART_CTS_PIN;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(USART_CTS_GPIO, &GPIO_InitStruct);
        #endif

        #ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_RTS
        /* RTS configuration */
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStruct.GPIO_Pin = USART_RTS_PIN;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(USART_RTS_GPIO, &GPIO_InitStruct);
        #endif

        /* 3. Configure USART clock (for synchronous mode) */
        #ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_SYNC
	USART_ClockInitTypeDef USART_ClockInitStruct;
        USART_ClockInitStruct.USART_Clock = USART_Clock_Enable;
        USART_ClockInitStruct.USART_CPOL = USART_CLOCK_POL;
        USART_ClockInitStruct.USART_CPHA = USART_CLOCK_PHASE;
        USART_ClockInitStruct.USART_LastBit = USART_CLOCK_LAST;
        USART_ClockInit(USART_REG, &USART_ClockInitStruct);
        #endif

        /* 4. Configure USART */
	USART_InitStruct.USART_BaudRate = CONFIG_LIB_EARLYCON_STM32_SERIAL_BAUDRATE; 
	USART_InitStruct.USART_WordLength = USART_WordLength_8b; 
	USART_InitStruct.USART_StopBits = USART_STOP;
	USART_InitStruct.USART_Parity = USART_PARITY;
	USART_InitStruct.USART_HardwareFlowControl = USART_HW; 
	USART_InitStruct.USART_Mode = USART_MODE;
	USART_Init(USART_REG, &USART_InitStruct);

	USART_Cmd(USART_REG, ENABLE); 
}


void stm32f10x_usart_putchar(char c)
{
#ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_TX
	while(!(USART_REG->SR & USART_SR_TC)); 
	USART_REG->DR = c; 
#endif
}

int stm32f10x_usart_getchar() {
#ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_RX
	while (!(USART_REG->SR & USART_SR_RXNE));
	return (int) USART_REG->DR;
#else
        return 0;
#endif
}


int stm32f10x_usart_avail() {
#ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_RX
	return (USART_REG->SR & USART_SR_RXNE);
#else
        return 0;
#endif
}


struct early_console g_early_console = {
	.rxchar = stm32f10x_usart_getchar,
	.txchar = stm32f10x_usart_putchar,
	.init = stm32f10x_usart_init,		
	.havechar = stm32f10x_usart_avail,
};
