#Suck in the MCU database
include src/arch/avr/mcu_database.mk
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature
HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings
CFLAGS+=-mmcu=$(MCU) -DF_CPU=$(CONFIG_F_CPU) 
LDFLAGS+=-mmcu=$(MCU) -DF_CPU=$(CONFIG_F_CPU)

#Avr needs hex and eep
TARGET_HEXFILE=$(IMAGES_DIR)$(TARGET).hex
TARGET_EEPFILE=$(IMAGES_DIR)$(TARGET).eep
OUTPUT_TARGETS+=$(TARGET_HEXFILE) $(TARGET_EEPFILE)
