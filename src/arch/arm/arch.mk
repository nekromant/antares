# We do need a custom LD file here

GCC_LDFILE=$(TMPDIR)/ldfile.lds
LD_NO_COMBINE=y


#Set our build goals
BUILDGOALS=$(IMAGENAME).bin $(IMAGENAME).lss

ifeq ($(CONFIG_MCU_STM32),y)
include $(ANTARES_DIR)/src/arch/arm/stm32/arch.mk
endif
