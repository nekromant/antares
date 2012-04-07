#TODO: ST's frequency init SUCKS
#We need a better way to set it up
ifeq ($(CONFIG_STM32_FREQ_72),y)
CFLAGS+=-DSYSCLK_FREQ_72MHz=72000000
endif

ifeq ($(CONFIG_STM32_FREQ_56),y)
CFLAGS+=-DSYSCLK_FREQ_56MHz=56000000
endif

ifeq ($(CONFIG_STM32_FREQ_48),y)
CFLAGS+=-DSYSCLK_FREQ_48MHz=48000000
endif

ifeq ($(CONFIG_STM32_FREQ_36),y)
CFLAGS+=-DSYSCLK_FREQ_36MHz=36000000
endif

ifeq ($(CONFIG_STM32_FREQ_24),y)
CFLAGS+=-DSYSCLK_FREQ_24MHz=24000000
endif

ifeq ($(CONFIG_STM32_FREQ_HSE),y)
CFLAGS+=-DSYSCLK_FREQ_HSE=HSE_VALUE
endif

CFLAGS+=-DHSE_VALUE=$(CONFIG_STM32_HSE_VALUE)
objects-y+=core_cm3.o
objects-y+=misc.o
objects-$(CONFIG_STM32_LIB_ADC)+=stm32f10x_adc.o
objects-$(CONFIG_STM32_LIB_BKP)+=stm32f10x_bkp.o
objects-$(CONFIG_STM32_LIB_CAN)+=stm32f10x_can.o
objects-$(CONFIG_STM32_LIB_CEC)+=stm32f10x_cec.o
objects-$(CONFIG_STM32_LIB_CRC)+=stm32f10x_crc.o
objects-$(CONFIG_STM32_LIB_DAC)+=stm32f10x_dac.o
objects-$(CONFIG_STM32_LIB_DBGMCU)+=stm32f10x_dbgmcu.o
objects-$(CONFIG_STM32_LIB_DMA)+=stm32f10x_dma.o
objects-$(CONFIG_STM32_LIB_EXTI)+=stm32f10x_exti.o
objects-$(CONFIG_STM32_LIB_FLASH)+=stm32f10x_flash.o
objects-$(CONFIG_STM32_LIB_FSMC)+=stm32f10x_fsmc.o
objects-$(CONFIG_STM32_LIB_GPIO)+=stm32f10x_gpio.o
objects-$(CONFIG_STM32_LIB_I2C)+=stm32f10x_i2c.o
objects-$(CONFIG_STM32_LIB_IWDG)+=stm32f10x_iwdg.o
objects-$(CONFIG_STM32_LIB_PWR)+=stm32f10x_pwr.o
objects-$(CONFIG_STM32_LIB_RCC)+=stm32f10x_rcc.o
objects-$(CONFIG_STM32_LIB_RTC)+=stm32f10x_rtc.o
objects-$(CONFIG_STM32_LIB_SDIO)+=stm32f10x_sdio.o
objects-$(CONFIG_STM32_LIB_SPI)+=stm32f10x_spi.o
objects-$(CONFIG_STM32_LIB_TIM)+=stm32f10x_tim.o
objects-$(CONFIG_STM32_LIB_USART)+=stm32f10x_usart.o
objects-$(CONFIG_STM32_LIB_WWDG)+=stm32f10x_wwdg.o
objects-$(CONFIG_STM32_LIB_SYSTEM)+=system_stm32f10x.o
