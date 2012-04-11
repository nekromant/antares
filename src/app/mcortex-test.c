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

#include "misc.h"

void mdelay(int n)
{
        int i = n*10;													/* About 1/4 second delay */
        while (i-- > 0) {
                asm("nop");													/* This stops it optimising code out */
        }
}

//TODO: Tuning to custom freqs
void setup_freq()
{
  //Tune up to 76Mhz
  /* enable HSE */
  RCC->CR = RCC->CR | 0x00010001;
  while ((RCC->CR & 0x00020000) == 0); /* for it to come on */
  
  /* enable flash prefetch buffer */
  FLASH->ACR = 0x00000012;

  /* Configure PLL */
  RCC->CFGR = RCC->CFGR | 0x00110400;  /* pll=72Mhz,APB1=36Mhz,AHB=72Mhz */
  RCC->CR = RCC->CR     | 0x01000000;  /* enable the pll */
  while ((RCC->CR & 0x03000000) == 0);         /* wait for it to come on */
  
  /* Set SYSCLK as PLL */
  RCC->CFGR = RCC->CFGR | 0x00000002;
  while ((RCC->CFGR & 0x00000008) == 0); /* wait for it to come on */
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


void SysTick_Handler()
{
    tmgr_tick();
}





//TODO: Totally rewrite it once a portable driver arrives
#define sFLASH_SPI                       SPI1
#define sFLASH_SPI_CLK                   RCC_APB2Periph_SPI1
#define sFLASH_SPI_SCK_PIN               GPIO_Pin_5                  /* PA.05 */
#define sFLASH_SPI_SCK_GPIO_PORT         GPIOA                       /* GPIOA */
#define sFLASH_SPI_SCK_GPIO_CLK          RCC_APB2Periph_GPIOA
#define sFLASH_SPI_MISO_PIN              GPIO_Pin_6                  /* PA.06 */
#define sFLASH_SPI_MISO_GPIO_PORT        GPIOA                       /* GPIOA */
#define sFLASH_SPI_MISO_GPIO_CLK         RCC_APB2Periph_GPIOA
#define sFLASH_SPI_MOSI_PIN              GPIO_Pin_7                  /* PA.07 */
#define sFLASH_SPI_MOSI_GPIO_PORT        GPIOA                       /* GPIOA */
#define sFLASH_SPI_MOSI_GPIO_CLK         RCC_APB2Periph_GPIOA
void mcortex_flash_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*!< sFLASH_SPI_CS_GPIO, sFLASH_SPI_MOSI_GPIO, sFLASH_SPI_MISO_GPIO 
       and sFLASH_SPI_SCK_GPIO Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);

  /*!< sFLASH_SPI Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  
  /*!< Configure sFLASH_SPI pins: SCK */
  GPIO_InitStructure.GPIO_Pin = sFLASH_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(sFLASH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure sFLASH_SPI pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = sFLASH_SPI_MOSI_PIN;
  GPIO_Init(sFLASH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure sFLASH_SPI pins: MISO */
  GPIO_InitStructure.GPIO_Pin = sFLASH_SPI_MISO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_Init(sFLASH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
  
  /*!< Configure sFLASH_CS_PIN pin: sFLASH Card CS pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB,GPIO_Pin_1|GPIO_Pin_0);
}

#define sFLASH_M25P128_ID         0x202018
#define sFLASH_M25P128_ID         0x202018
#define sFLASH_M25P32_ID          0x202016
#include "mcortex-sscu.h"

struct spi_flash_chip 
{
	char* name;
	uint32_t id;
	uint32_t pagesize;
	uint32_t erase_size;
	uint32_t size;
};

static struct spi_flash_chip sflash =
{
	.name = "ST M25P32",
	.id = 0x202016,
	.pagesize = 256,
	.erase_size = 512*1024,
	.size = 4 * 1024* 1024
};


int main()
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
		
		Usart1Init();
		SysTick_Config(SystemCoreClock / 100);
		setvbuf(stdin, NULL, _IONBF, 0);
		setvbuf(stdout, NULL, _IONBF, 0);
		setvbuf(stderr, NULL, _IONBF, 0);
		printf("\xFF\n\r\n\r"); //text mode
		printk("Motor Cortex rev. 0.9 :: Antares uC system, v. %s\n\r", CONFIG_VERSION_STRING);
		printk("git commit: %s\n\r", CONFIG_VERSION_GIT);
		mcortex_flash_init();
		printk("sflash: lowlevel init performed\n\r");
		sFLASH_Init();
		uint32_t id = sFLASH_ReadID();
		printk("sflash: Detected %s (0x%x) %d bytes\n\r", sflash.name, sflash.id, sflash.size);
		mcortex_fpga_init();
		printk("bootmode: press 'f' to update fpga firmware in 1 sec\n\r");
		char m = getchar();
		if ('f' == m)
		{
			mcortex_fpga_update();
		}
// 		printk("sflash: erasing everything...\n\r");
// 		
// 		printk("sflash: done\n\r");
		int t = tmgr_get_uptime();
		mcortex_fpga_fromspi();
		mcortex_fpga_smc();
		mctx_init();
		t = tmgr_get_uptime() - t;
		printk("fpga: configuration done in %d ticks\n\r",t);
		// TODO: CODE HERE!!!
		while(1)
		{
// 			printk("tick!\n\r");
			mdelay(10000);
		}
        return 0;
}


