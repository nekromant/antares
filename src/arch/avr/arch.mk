include $(SRCDIR)/src/arch/avr/mcu_database.mk
#Set our build goals
BUILDGOALS=$(IMAGENAME).bin $(IMAGENAME).lss $(IMAGENAME).hex $(IMAGENAME).eep

# Do not combine objects into built-in.o
# This screws up things on avr and breaks ANTARES_* macros
# So it's 100% safe to set this to y
LD_NO_COMBINE=y

TARGET_HEXFILE=$(IMAGENAME).hex
TARGET_BINFILE=$(IMAGENAME).bin
TARGET_EEPFILE=$(IMAGENAME).eep



CFLAGS+= -mmcu=$(MCU) -DF_CPU=$(CONFIG_F_CPU)
ELFFLAGS+= -mmcu=$(MCU) -DF_CPU=$(CONFIG_F_CPU)

#avr-specifik voodoo for hex generation

HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature
HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


%.hex: %.elf
		$(SILENT_OBJCOPY) $(OBJCOPY) -O ihex $(HEX_FLASH_FLAGS)  $< $@	
		$(call colorize,$(t_cyn))
		@echo "Created Intel HEX file: $(@)"
		$(call colorize,$(col_rst))

%.eep: %.elf
		$(SILENT_OBJCOPY) $(OBJCOPY) $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0
		$(call colorize,$(t_cyn))
		@echo "Created Intel HEX file for eeprom: $(@)"
		$(call colorize,$(col_rst))
