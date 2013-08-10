include $(ANTARES_DIR)/src/arch/pic32/mcu_database.mk
#Set our build goals
BUILDGOALS=$(IMAGENAME).lss $(IMAGENAME).hex

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

#FLASHSIZE= $(shell echo $$((`echo -e "\#include <bmx.h>\nFLASH_SZ" | avr-cpp -mmcu=$(MCU) | sed '$$!d'` + 1 $(APPSIZE) )))


#FLASHSIZE= $(shell echo $$((`echo -e "\#include <avr/io.h>\nFLASHEND" | avr-cpp -mmcu=$(MCU) | sed '$$!d'` + 1 $(APPSIZE) )))
#RAMSTART= $(shell echo $$((`echo -e "\#include <avr/io.h>\nRAMSTART" | avr-cpp -mmcu=$(MCU) | sed '$$!d'`)))
#RAMEND=$(shell echo $$((`echo -e "\#include <avr/io.h>\nRAMEND" | avr-cpp -mmcu=$(MCU) | sed '$$!d'` )))
#RAMSIZE=$(shell echo $$(($(RAMEND)-$(RAMSTART) + 1 )))
#EESIZE= $(shell echo $$((`echo -e "\#include <avr/io.h>\nE2END" | avr-cpp -mmcu=$(MCU) | sed '$$!d'` + 1 )))



%.hex: %.elf
		$(SILENT_OBJCOPY) $(OBJCOPY) -O ihex $(HEX_FLASH_FLAGS)  $< $@	

%.eep: %.elf
		$(SILENT_OBJCOPY) $(OBJCOPY) $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.eep.bin: %.elf
		$(SILENT_OBJCOPY) $(OBJCOPY) $(HEX_EEPROM_FLAGS) -O binary $< $@ || exit 0


sizecheck: $(filter-out sizecheck,$(BUILDGOALS))
	$(Q) echo "Not yet implemented!"

#	$(Q)$(ANTARES_DIR)/scripts/meter "$(FBANNER)" \
#	`$(SIZE) $(IMAGENAME).elf |grep elf|awk '{print $$1+$$2}'` \
#	$(FLASHSIZE);
#	$(Q)$(ANTARES_DIR)/scripts/meter "EEPROM Usage" \
#	`$(STAT) $(IMAGENAME).eep.bin -c %s` \
#	$(EESIZE);
#	$(Q)$(ANTARES_DIR)/scripts/meter "RAM Usage" \
#	`$(SIZE) $(IMAGENAME).elf |grep elf|awk '{print $$2+$$3}'` \
#	$(RAMSIZE);
#	$(Q)echo "Note: Ram usage is only rough minimum estimation (.data + .bss)"

#list-interrupts:
#	echo "#include <avr/interrupt.h>" | $(TOOL_PREFIX)gcc -mmcu=$(MCU) -dM -E -|grep "_vect" | grep -v "_num" | awk '{printf $$2"\n"}'


BUILDGOALS+=sizecheck
PHONY+=sizecheck
