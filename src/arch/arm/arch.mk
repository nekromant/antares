LD_NO_COMBINE=y
#Do not use linux-like compile
BUILDGOALS=$(IMAGENAME).bin $(IMAGENAME).lss

ARCH_FEATURES:=ANTARES_STARTUP

ifeq ($(CONFIG_MCU_STM32),y)
include $(ANTARES_DIR)/src/arch/arm/stm32/arch.mk
endif
