#TODO: Rest of this shit

ifeq ($(CONFIG_STM32F10X_LD),y)
	CFLAGS+=-DSTM32F10X_LD
else ifeq ($(CONFIG_STM32F10X_LD_VL),y)
	CFLAGS+=-DSTM32F10X_LD_VL
else ifeq ($(CONFIG_STM32F10X_MD),y)
	CFLAGS+=-DSTM32F10X_MD
else ifeq ($(CONFIG_STM32F10X_HD),y)
	CFLAGS+=-DSTM32F10X_HD
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
CFLAGS+=-I$(SRCDIR)/src/arch/arm/stm32/include
CFLAGS+=-include $(SRCDIR)/src/arch/arm/stm32/include/assert.h

$(TMPDIR)/ldfile.lds:
	$(SILENT_GEN) ESTACK=$(CONFIG_ARM_ESTACK) \
		FLASH_SZ=$(CONFIG_ARM_FLASHSZ) \
		FLASH_BASE=$(CONFIG_ARM_FLASHBASE) \
		RAM_BASE=$(CONFIG_ARM_RAMBASE) \
		RAM_SZ=$(CONFIG_ARM_RAMSZ) \
		MIN_STACK=$(CONFIG_ARM_MINSTACK) \
		MIN_HEAP=$(CONFIG_ARM_MINHEAP) \
		$(SRCDIR)/ldfiles/stm32_flash.ld.sh > $(@)

