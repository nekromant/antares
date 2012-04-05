//#define STM32F10X_LD
#include <arch/antares.h>
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

void mdelay(int n)
{
        int i = n*100;													/* About 1/4 second delay */
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




#include "mcortex-sscu.h"
int main()
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
		mcortex_fpga_init();
		mcortex_fpga_fromflash();
		while(1);;
        return 0;
}


