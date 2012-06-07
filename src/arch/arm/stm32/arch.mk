#TODO: Rest of this shit

ifeq ($(CONFIG_STM32F10X_LD),y)
	CFLAGS+=-DSTM32F10X_LD
else ifeq ($(CONFIG_STM32F10X_LD_VL),y)
	CFLAGS+=-DSTM32F10X_LD_VL
else ifeq ($(CONFIG_STM32F10X_MD),y)
	CFLAGS+=-DSTM32F10X_MD
else ifeq ($(CONFIG_STM32F10X_MD_VL),y)
	CFLAGS+=-DSTM32F10X_MD_VL	
else ifeq ($(CONFIG_STM32F10X_HD),y)
	CFLAGS+=-DSTM32F10X_HD
else ifeq ($(CONFIG_STM32F10X_HD_VL),y)
	CFLAGS+=-DSTM32F10X_HD_VL
else ifeq ($(CONFIG_STM32F10X_XL),y)
	CFLAGS+=-DSTM32F10X_XL
else ifeq ($(CONFIG_STM32F10X_CL),y)
	CFLAGS+=-DSTM32F10X_CL
endif

ifeq ($(CONFIG_STM32_FULL_ASSERT),y)
	CFLAGS+=-DFULL_ASSERT
endif

#FixMe: ...
CFLAGS+=-fno-common -mcpu=cortex-m3 -mthumb
ASFLAGS+=-fno-common -mcpu=cortex-m3 -mthumb

#FixMe: ...
CFLAGS+=-I$(ANTARES_DIR)/src/arch/arm/stm32/include
CFLAGS+=-include $$(ANTARES_DIR)/src/arch/arm/stm32/include/assert.h

# Let the magic of gcc preprocessor commence!
$(TMPDIR)/ldfile.lds: $(GCC_LDFILE_IN)
	cat "$^" | $(SILENT_GEN) $(CC) -E -P -include $(TOPDIR)/include/generated/autoconf.h -include $(ANTARES_DIR)/include/lib/sizes.h - > $(@)

