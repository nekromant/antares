#TODO: ST's frequency init SUCKS
#We need a better way to set it up
ifeq ($(CONFIG_STM32_FREQ_HSE),y)
CFLAGS+=-DSYSCLK_FREQ_HSE=HSE_VALUE
endif

CFLAGS+=-DHSE_VALUE=$(CONFIG_STM32_HSE_VALUE)
objects-y+=core_cm3.o
objects-y+=misc.o
objects-$(CONFIG_STM32_LIB_ADC)+=stm32l1xx_adc.o
objects-$(CONFIG_STM32_LIB_BKP)+=stm32l1xx_bkp.o
objects-$(CONFIG_STM32_LIB_CAN)+=stm32l1xx_can.o
objects-$(CONFIG_STM32_LIB_CEC)+=stm32l1xx_cec.o
objects-$(CONFIG_STM32_LIB_CRC)+=stm32l1xx_crc.o
objects-$(CONFIG_STM32_LIB_DAC)+=stm32l1xx_dac.o
objects-$(CONFIG_STM32_LIB_DBGMCU)+=stm32l1xx_dbgmcu.o
objects-$(CONFIG_STM32_LIB_DMA)+=stm32l1xx_dma.o
objects-$(CONFIG_STM32_LIB_EXTI)+=stm32l1xx_exti.o
objects-$(CONFIG_STM32_LIB_FLASH)+=stm32l1xx_flash.o
objects-$(CONFIG_STM32_LIB_FSMC)+=stm32l1xx_fsmc.o
objects-$(CONFIG_STM32_LIB_GPIO)+=stm32l1xx_gpio.o
objects-$(CONFIG_STM32_LIB_I2C)+=stm32l1xx_i2c.o
objects-$(CONFIG_STM32_LIB_IWDG)+=stm32l1xx_iwdg.o
objects-$(CONFIG_STM32_LIB_PWR)+=stm32l1xx_pwr.o
objects-$(CONFIG_STM32_LIB_RCC)+=stm32l1xx_rcc.o
objects-$(CONFIG_STM32_LIB_RTC)+=stm32l1xx_rtc.o
objects-$(CONFIG_STM32_LIB_SDIO)+=stm32l1xx_sdio.o
objects-$(CONFIG_STM32_LIB_SPI)+=stm32l1xx_spi.o
objects-$(CONFIG_STM32_LIB_TIM)+=stm32l1xx_tim.o
objects-$(CONFIG_STM32_LIB_USART)+=stm32l1xx_usart.o
objects-$(CONFIG_STM32_LIB_WWDG)+=stm32l1xx_wwdg.o
objects-$(CONFIG_STM32_LIB_SYSTEM)+=system_stm32l1xx.o
