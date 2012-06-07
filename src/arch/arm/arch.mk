# We do need a custom LD file here
# There's a nug in LD: it can't use symbols in memory sections, 
# therefore we can't calc the offsets from kconfig at the runtime.
# I solved that by running gcc preprocessor against an ld file template
GCC_LDFILE_IN=$(ANTARES_DIR)/src/arch/arm/stm32/generic.lds
GCC_LDFILE=$(TMPDIR)/ldfile.lds
LD_NO_COMBINE=y


#Set our build goals
BUILDGOALS=$(IMAGENAME).bin $(IMAGENAME).lss

ifeq ($(CONFIG_MCU_STM32),y)
include $(ANTARES_DIR)/src/arch/arm/stm32/arch.mk
endif
