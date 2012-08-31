include $(ANTARES_DIR)/src/arch/msp430/mcu_database.mk
#Set our build goals
BUILDGOALS=$(IMAGENAME).bin $(IMAGENAME).lss

# Do not combine objects into built-in.o
# Failing to do so screws up things and breaks ANTARES_* macros
# So it's 100% safe to set this to y
LD_NO_COMBINE=y

TARGET_BINFILE=$(IMAGENAME).bin
TARGET_EEPFILE=$(IMAGENAME).eep

CFLAGS+= -mmcu=$(MCU)
ELFFLAGS+= -mmcu=$(MCU)

%.hex: %.elf
		$(SILENT_OBJCOPY) $(OBJCOPY) -O ihex $(HEX_FLASH_FLAGS)  $< $@	
		$(call colorize,$(t_cyn))
		@echo "Created Intel HEX file: $(@)"
		$(call colorize,$(col_rst))
