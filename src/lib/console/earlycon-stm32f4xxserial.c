#include <lib/earlycon.h>
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"

/* Hardware configuration constants */

#ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_PORT_USART1
        #define USART_PERIPH RCC_APB2Periph_USART1
        #define USART_REG USART1
        #define USART_AF GPIO_AF_USART1
        
        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_1_TX_1)
                #define USART_TX_PS GPIO_PinSource9
                #define USART_TX_PIN GPIO_Pin_9
                #define USART_TX_GPIO GPIOA
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_1_TX_2)
                #define USART_TX_PS GPIO_PinSource6
                #define USART_TX_PIN GPIO_Pin_6
                #define USART_TX_GPIO GPIOB
        #endif
                
        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_1_RX_1)
                #define USART_RX_PS GPIO_PinSource10
                #define USART_RX_PIN GPIO_Pin_10
                #define USART_RX_GPIO GPIOA
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_1_RX_2)
                #define USART_RX_PS GPIO_PinSource7
                #define USART_RX_PIN GPIO_Pin_7
                #define USART_RX_GPIO GPIOB
        #endif
                
        #define USART_CK_PS GPIO_PinSource8
        #define USART_CK_PIN GPIO_Pin_8
        #define USART_CK_GPIO GPIOA

        #define USART_CTS_PS GPIO_PinSource11
        #define USART_CTS_PIN GPIO_Pin_11
        #define USART_CTS_GPIO GPIOA
        
        #define USART_RTS_PS GPIO_PinSource12
        #define USART_RTS_PIN GPIO_Pin_12
        #define USART_RTS_GPIO GPIOA

        #define GPIO_PERIPH (RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB)
#endif

#ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_PORT_USART2
        #define USART_PERIPH RCC_APB1Periph_USART2
        #define USART_REG USART2
        #define USART_AF GPIO_AF_USART2

        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_2_TX_1)
                #define USART_TX_PS GPIO_PinSource2
                #define USART_TX_PIN GPIO_Pin_2
                #define USART_TX_GPIO GPIOA
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_2_TX_2)
                #define USART_TX_PS GPIO_PinSource5
                #define USART_TX_PIN GPIO_Pin_5
                #define USART_TX_GPIO GPIOD
        #endif
                
        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_2_RX_1)
                #define USART_RX_PS GPIO_PinSource3  
                #define USART_RX_PIN GPIO_Pin_3
                #define USART_RX_GPIO GPIOA
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_2_RX_2)
                #define USART_RX_PS GPIO_PinSource6
                #define USART_RX_PIN GPIO_Pin_6
                #define USART_RX_GPIO GPIOD
        #endif
                
        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_2_CK_1)
                #define USART_CK_PS GPIO_PinSource4
                #define USART_CK_PIN GPIO_Pin_4
                #define USART_CK_GPIO GPIOA
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_2_CK_2)
                #define USART_CK_PS GPIO_PinSource7
                #define USART_CK_PIN GPIO_Pin_7
                #define USART_CK_GPIO GPIOD
        #endif

        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_2_CTS_1)
                #define USART_CTS_PS GPIO_PinSource0
                #define USART_CTS_PIN GPIO_Pin_0
                #define USART_CTS_GPIO GPIOA
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_2_CTS_2)
                #define USART_CTS_PS GPIO_PinSource3
                #define USART_CTS_PIN GPIO_Pin_3
                #define USART_CTS_GPIO GPIOD
        #endif

        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_2_RTS_1)
                #define USART_RTS_PS GPIO_PinSource1
                #define USART_RTS_PIN GPIO_Pin_1
                #define USART_RTS_GPIO GPIOA
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_2_RTS_2)
                #define USART_RTS_PS GPIO_PinSource4
                #define USART_RTS_PIN GPIO_Pin_4
                #define USART_RTS_GPIO GPIOD
        #endif

                
        #define GPIO_PERIPH (RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOD)
#endif

#ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_PORT_USART3
        #define USART_PERIPH RCC_APB1Periph_USART3
        #define USART_REG USART3
        #define USART_AF GPIO_AF_USART3

        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_3_TX_1)
                #define USART_TX_PS GPIO_PinSource10
                #define USART_TX_PIN GPIO_Pin_10
                #define USART_TX_GPIO GPIOB
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_3_TX_2)
                #define USART_TX_PS GPIO_PinSource8
                #define USART_TX_PIN GPIO_Pin_8
                #define USART_TX_GPIO GPIOD
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_3_TX_3)
                #define USART_TX_PS GPIO_PinSource10
                #define USART_TX_PIN GPIO_Pin_10
                #define USART_TX_GPIO GPIOC
        #endif
                
        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_3_RX_1)
                #define USART_RX_PS GPIO_PinSource11
                #define USART_RX_PIN GPIO_Pin_11
                #define USART_RX_GPIO GPIOB
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_3_RX_2)
                #define USART_RX_PS GPIO_PinSource9
                #define USART_RX_PIN GPIO_Pin_9
                #define USART_RX_GPIO GPIOD
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_3_RX_3)
                #define USART_RX_PS GPIO_PinSource11
                #define USART_RX_PIN GPIO_Pin_11
                #define USART_RX_GPIO GPIOC
        #endif
                
        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_3_CK_1)
                #define USART_CK_PS GPIO_PinSource12
                #define USART_CK_PIN GPIO_Pin_12
                #define USART_CK_GPIO GPIOB
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_3_CK_2)
                #define USART_CK_PS GPIO_PinSource10
                #define USART_CK_PIN GPIO_Pin_10
                #define USART_CK_GPIO GPIOD
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_3_CK_3)
                #define USART_CK_PS GPIO_PinSource12
                #define USART_CK_PIN GPIO_Pin_12
                #define USART_CK_GPIO GPIOC
        #endif

        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_3_CTS_1)
                #define USART_CTS_PS GPIO_PinSource13
                #define USART_CTS_PIN GPIO_Pin_13
                #define USART_CTS_GPIO GPIOB
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_3_CTS_2)
                #define USART_CTS_PS GPIO_PinSource11
                #define USART_CTS_PIN GPIO_Pin_11
                #define USART_CTS_GPIO GPIOD
        #endif

        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_3_RTS_1)
                #define USART_RTS_PS GPIO_PinSource14
                #define USART_RTS_PIN GPIO_Pin_14
                #define USART_RTS_GPIO GPIOB
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_3_RTS_2)
                #define USART_RTS_PS GPIO_PinSource12
                #define USART_RTS_PIN GPIO_Pin_12
                #define USART_RTS_GPIO GPIOD
        #endif

        #define GPIO_PERIPH (RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD)
#endif

#ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_PORT_UART4
        #define USART_PERIPH RCC_APB1Periph_UART4
        #define USART_REG UART4
        #define USART_AF GPIO_AF_UART4

        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_4_TX_1)
                #define USART_TX_PS GPIO_PinSource10
                #define USART_TX_PIN GPIO_Pin_10
                #define USART_TX_GPIO GPIOC
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_4_TX_2)
                #define USART_TX_PS GPIO_PinSource0
                #define USART_TX_PIN GPIO_Pin_0
                #define USART_TX_GPIO GPIOA
        #endif
                
        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_4_RX_1)
                #define USART_RX_PS GPIO_PinSource11
                #define USART_RX_PIN GPIO_Pin_11
                #define USART_RX_GPIO GPIOC
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_4_RX_2)
                #define USART_RX_PS GPIO_PinSource1
                #define USART_RX_PIN GPIO_Pin_1
                #define USART_RX_GPIO GPIOA
        #endif
                
        #define GPIO_PERIPH (RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC)
#endif

#ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_PORT_UART5
        #define USART_PERIPH RCC_APB1Periph_UART5
        #define USART_REG UART5
        #define USART_AF GPIO_AF_UART5
        
        #define USART_TX_PS GPIO_PinSource12
        #define USART_TX_PIN GPIO_Pin_12
        #define USART_TX_GPIO GPIOC

        #define USART_RX_PS GPIO_PinSource2
        #define USART_RX_PIN GPIO_Pin_2
        #define USART_RX_GPIO GPIOD
        
        #define GPIO_PERIPH (RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD)
#endif

#ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_PORT_USART6
        #define USART_PERIPH RCC_APB2Periph_USART6
        #define USART_REG USART6
        #define USART_AF GPIO_AF_USART6

        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_6_TX_1)
                #define USART_TX_PS GPIO_PinSource6
                #define USART_TX_PIN GPIO_Pin_6
                #define USART_TX_GPIO GPIOC
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_6_TX_2)
                #define USART_TX_PS GPIO_PinSource14
                #define USART_TX_PIN GPIO_Pin_14
                #define USART_TX_GPIO GPIOG
        #endif
                
        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_6_RX_1)
                #define USART_RX_PS GPIO_PinSource7
                #define USART_RX_PIN GPIO_Pin_7
                #define USART_RX_GPIO GPIOC
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_6_RX_2)
                #define USART_RX_PS GPIO_PinSource9
                #define USART_RX_PIN GPIO_Pin_9
                #define USART_RX_GPIO GPIOG
        #endif
                
        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_6_CK_1)
                #define USART_CK_PS GPIO_PinSource7
                #define USART_CK_PIN GPIO_Pin_7
                #define USART_CK_GPIO GPIOG
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_6_CK_2)
                #define USART_CK_PS GPIO_PinSource8
                #define USART_CK_PIN GPIO_Pin_8
                #define USART_CK_GPIO GPIOC
        #endif

        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_6_CTS_1)
                #define USART_CTS_PS GPIO_PinSource15
                #define USART_CTS_PIN GPIO_Pin_15
                #define USART_CTS_GPIO GPIOG
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_6_CTS_2)
                #define USART_CTS_PS GPIO_PinSource13
                #define USART_CTS_PIN GPIO_Pin_13
                #define USART_CTS_GPIO GPIOG
        #endif

        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_6_RTS_1)
                #define USART_RTS_PS GPIO_PinSource8
                #define USART_RTS_PIN GPIO_Pin_8
                #define USART_RTS_GPIO GPIOG
        #elif defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_6_RTS_2)
                #define USART_RTS_PS GPIO_PinSource12
                #define USART_RTS_PIN GPIO_Pin_12
                #define USART_RTS_GPIO GPIOG
        #endif

                
        #define GPIO_PERIPH (RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOG)
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

void stm32f4xx_uart_init() {
	GPIO_InitTypeDef    GPIO_InitStruct;
	USART_InitTypeDef    USART_InitStruct;

        /* 0. Configure RCC */
        RCC_AHB1PeriphClockCmd(GPIO_PERIPH, ENABLE);
        #if defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_PORT_USART1) || defined(CONFIG_LIB_EARLYCON_STM32_SERIAL_PORT_USART6)
        RCC_APB2PeriphClockCmd(USART_PERIPH, ENABLE);
        #else
        RCC_APB1PeriphClockCmd(USART_PERIPH, ENABLE);
        #endif

        /* 1. Configure GPIO */
        #ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_TX
        /* TX configuration */
        GPIO_PinAFConfig(USART_TX_GPIO, USART_TX_PS, USART_AF);
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStruct.GPIO_Pin = USART_TX_PIN;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
        GPIO_Init(USART_TX_GPIO, &GPIO_InitStruct);
        #endif

        #ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_RX
        /* RX configuration */
        GPIO_PinAFConfig(USART_RX_GPIO, USART_RX_PS, USART_AF);
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStruct.GPIO_Pin = USART_RX_PIN;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
        GPIO_Init(USART_RX_GPIO, &GPIO_InitStruct);
        #endif

        #ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_SYNC
        /* CK configuration */
        GPIO_PinAFConfig(USART_CK_GPIO, USART_CK_PS, USART_AF);
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStruct.GPIO_Pin = USART_CK_PIN;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
        GPIO_Init(USART_CK_GPIO, &GPIO_InitStruct);
        #endif

        #ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_CTS
        /* CTS configuration */
        GPIO_PinAFConfig(USART_CTS_GPIO, USART_CTS_PS, USART_AF);
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStruct.GPIO_Pin = USART_CTS_PIN;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
        GPIO_Init(USART_CTS_GPIO, &GPIO_InitStruct);
        #endif

        #ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_RTS
        /* RTS configuration */
        GPIO_PinAFConfig(USART_RTS_GPIO, USART_RTS_PS, USART_AF);
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStruct.GPIO_Pin = USART_RTS_PIN;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
        GPIO_Init(USART_RTS_GPIO, &GPIO_InitStruct);
        #endif

        /* 2. Configure USART clock (for synchronous mode) */
        #ifdef CONFIG_LIB_EARLYCON_STM32_SERIAL_SYNC
	USART_ClockInitTypeDef USART_ClockInitStruct;
        USART_ClockInitStruct.USART_Clock = USART_Clock_Enable;
        USART_ClockInitStruct.USART_CPOL = USART_CLOCK_POL;
        USART_ClockInitStruct.USART_CPHA = USART_CLOCK_PHASE;
        USART_ClockInitStruct.USART_LastBit = USART_CLOCK_LAST;
        USART_ClockInit(USART_REG, &USART_ClockInitStruct);
        #endif

        /* 3. Configure USART */
	USART_InitStruct.USART_BaudRate = CONFIG_LIB_EARLYCON_STM32_SERIAL_BAUDRATE; 
	USART_InitStruct.USART_WordLength = USART_WordLength_8b; 
	USART_InitStruct.USART_StopBits = USART_STOP;
	USART_InitStruct.USART_Parity = USART_PARITY;
	USART_InitStruct.USART_HardwareFlowControl = USART_HW; 
	USART_InitStruct.USART_Mode = USART_MODE;
	USART_Init(USART_REG, &USART_InitStruct);

	USART_Cmd(USART_REG, ENABLE); 
}


static void stm32f4xx_uart_putchar(char c)
{
	while(!(USART_REG->SR & USART_SR_TC)); 
	USART_REG->DR = c; 
}

static int stm32f4xx_uart_getchar() {
	while (!(USART_REG->SR & USART_SR_RXNE));
	return (int) USART_REG->DR;
}


static int stm32f4xx_uart_avail() {
	return (USART_REG->SR & USART_SR_RXNE);
}


struct early_console g_early_console = {
	.rxchar = stm32f4xx_uart_getchar,
	.txchar = stm32f4xx_uart_putchar,
	.init = stm32f4xx_uart_init,		
	.havechar = stm32f4xx_uart_avail,
};
