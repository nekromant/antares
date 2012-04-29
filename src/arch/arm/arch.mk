# We do need a custom LD file here

GCC_LDFILE=$(TMPDIR)/ldfile.lds


#Set our build goals
BUILDGOALS=$(IMAGENAME).bin $(IMAGENAME).lss

ifeq ($(CONFIG_MCU_STM32),y)
include $(SRCDIR)/src/arch/arm/stm32/arch.mk
endif
