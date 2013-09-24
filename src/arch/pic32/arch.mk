include $(ANTARES_DIR)/src/arch/pic32/mcu_database.mk
#Set our build goals
BUILDGOALS=$(IMAGENAME).lss $(IMAGENAME).hex

# Antares startup code is not yet ready
#ARCH_FEATURES:= ANTARES_STARTUP

# Do not combine objects into built-in.o
# This screws up things on avr and breaks ANTARES_* macros
# So it's 100% safe to set this to y
LD_NO_COMBINE=y

TARGET_HEXFILE=$(IMAGENAME).hex
TARGET_BINFILE=$(IMAGENAME).bin


CFLAGS+=-mno-smart-io  -w  -fno-exceptions  -ffunction-sections  -fdata-sections  -g  -mdebugger  -Wcast-align  -fno-short-double 

ELFFLAGS+=-mprocessor=$(MCU) -DF_CPU=$(CONFIG_F_CPU)
CFLAGS += -mprocessor=$(MCU) -DF_CPU=$(CONFIG_F_CPU) 

ifeq ($(CONFIG_USE_UNO32_LD),y)
GCC_LDFILE=$(ANTARES_INSTALL_DIR)/src/arch/pic32/chipKIT-UNO32-application-32MX320F128L.ld
endif

ifeq ($(CONFIG_USE_MAX32_LD),y)
GCC_LDFILE=$(ANTARES_INSTALL_DIR)/src/arch/pic32/chipKIT-MAX32-application-32MX795F512L.ld
endif

MCHIP_PATH=$(shell dirname `which $(CC)`)

%.hex: %.elf
		$(SILENT_OBJCOPY) $(OBJCOPY) -O ihex $(HEX_FLASH_FLAGS)  $< $@	

%.eep: %.elf
		$(SILENT_OBJCOPY) $(OBJCOPY) $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.eep.bin: %.elf
		$(SILENT_OBJCOPY) $(OBJCOPY) $(HEX_EEPROM_FLAGS) -O binary $< $@ || exit 0

FLASHSIZE=$(shell MCU=$(MCU);echo $${MCU:8:3}*1024|bc)

#TODO: Find a way to find out target RAM size

sizecheck: $(filter-out sizecheck,$(BUILDGOALS))
	$(Q)$(ANTARES_DIR)/scripts/meter "Flash usage" \
	`$(SIZE) $(IMAGENAME).elf |grep elf|awk '{print $$1+$$2}'` \
	$(FLASHSIZE);
	$(Q)echo "RAM Usage:" \
	`$(SIZE) $(IMAGENAME).elf |grep elf|awk '{print $$2+$$3}'` bytes 
	$(Q)echo "Note: Ram usage is only rough minimum estimation (.data + .bss)"


list-interrupts:
	@echo "Not implemented yet. Patches welcome"


BUILDGOALS+=sizecheck
PHONY+=sizecheck
