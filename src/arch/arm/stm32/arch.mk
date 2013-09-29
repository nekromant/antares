# We do need a custom LD file here
# There's a bug in LD: it can't use symbols in memory sections, 
# therefore we can't calc the offsets from kconfig at the runtime.
# I solved that by running gcc preprocessor against an ld file template

ARCH_FEATURES:=\
	ANTARES_STARTUP \
	NEWLIB

GCC_LDFILE_IN=$(ANTARES_DIR)/src/arch/arm/stm32/generic.lds
GCC_LDFILE=$(TMPDIR)/ldfile.lds

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
else ifeq ($(CONFIG_STM32F4X),y)
	CFLAGS+=-DSTM32F4X
endif

ifeq ($(CONFIG_STM32_FULL_ASSERT),y)
	CFLAGS+=-DFULL_ASSERT
endif

# Handle default code placement. This looks like the same for st32f1x and stm32f4x

ifneq ($(CONFIG_STM32_OVERRIDE_BASES),y)
	GFLAGS+=-DCONFIG_STM32_FLASH_BASE=0x08000000
	GFLAGS+=-DCONFIG_STM32_RAM_BASE=0x20000000
endif

ifneq ($(CONFIG_STM32_SET_OFFSETS),y)
	GFLAGS+=-DCONFIG_STM32_FLASH_OFFSET=0x0
	GFLAGS+=-DCONFIG_STM32_RAM_OFFSET=0x0
endif

#STM32F4x series have cortex-m4 inside
#TODO: asserts for STM32F4X
ifeq ($(CONFIG_STM32F1X),y)
CFLAGS+=-mcpu=cortex-m3 -mthumb
ASFLAGS+=-mcpu=cortex-m3 -mthumb
CFLAGS+=-I$(ANTARES_DIR)/src/arch/arm/stm32/include-f1x
CFLAGS+=-include $$(ANTARES_DIR)/src/arch/arm/stm32/include-f1x/assert.h
endif

ifeq ($(CONFIG_STM32F4X),y)
CFLAGS+=-mcpu=cortex-m4 -mthumb
ASFLAGS+=-mcpu=cortex-m4 -mthumb
CFLAGS+=-I$(ANTARES_DIR)/src/arch/arm/stm32/include-f4x
CFLAGS+=-include $$(ANTARES_DIR)/src/arch/arm/stm32/include-f4x/assert.h
endif


CFLAGS+=-fno-common 
ASFLAGS+=-fno-common  -xassembler-with-cpp
CFLAGS+=$(GFLAGS)



# Let the magic of gcc preprocessor commence!
$(TMPDIR)/ldfile.lds: $(GCC_LDFILE_IN) silentoldconfig
	$(SILENT_GEN) cat $(GCC_LDFILE_IN) | $(CC) -E -P -include $(TOPDIR)/include/generated/autoconf.h $(GFLAGS) -include $(ANTARES_DIR)/include/lib/sizes.h - > $(@)

list-interrupts:
	$(Q)$(MAKE) -f $(ANTARES_DIR)/src/arch/arm/stm32/tools.mk list-interrupts

probe:
	$(Q)$(MAKE) SHELL=$(SHELL) -f $(ANTARES_DIR)/src/arch/arm/stm32/tools.mk stm32probe

sizecheck: $(filter-out sizecheck,$(BUILDGOALS))
	$(Q)$(ANTARES_DIR)/scripts/meter "FLASH Usage" \
	`$(STAT) $(IMAGENAME).bin -c %s` \
	$(CONFIG_STM32_FLASH_LEN);
	$(Q)$(ANTARES_DIR)/scripts/meter "RAM Usage" \
	`$(SIZE) $(IMAGENAME).elf |grep elf|awk '{print $$2+$$3}'` \
	$(CONFIG_STM32_RAM_LEN);
	$(Q)echo "Note: Ram usage is only rough minimum estimation (.data + .bss)"



BUILDGOALS+=sizecheck
PHONY+=list-interrupts probe sizecheck
