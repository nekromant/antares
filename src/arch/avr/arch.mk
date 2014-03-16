include $(ANTARES_DIR)/src/arch/avr/mcu_database.mk

ARCH_FEATURES:=ANTARES_STARTUP

#Set our build goals
BUILDGOALS=$(IMAGENAME).bin $(IMAGENAME).lss $(IMAGENAME).hex \
	$(IMAGENAME).eep $(IMAGENAME).eep.bin

# Do not combine objects into built-in.o
# This screws up things on avr and breaks ANTARES_* macros
# So it's 100% safe to set this to y
LD_NO_COMBINE=y

TARGET_HEXFILE=$(IMAGENAME).hex
TARGET_BINFILE=$(IMAGENAME).bin
TARGET_EEPFILE=$(IMAGENAME).eep
TARGET_EEPFILE=$(IMAGENAME).eep.bin

CFLAGS+= -mmcu=$(MCU) -DF_CPU=$(CONFIG_F_CPU)
LDFLAGS+= -mmcu=$(MCU) -DF_CPU=$(CONFIG_F_CPU)
ELFFLAGS+= -mmcu=$(MCU) -DF_CPU=$(CONFIG_F_CPU)

#avr-specifik voodoo for hex generation

#ELFFLAGS+=-Wl,--relax,--gc-sections 
#CFLAGS+=-Wl,--relax,--gc-sections 

ifeq ($(CONFIG_AVR_BLDR),y)
ELFFLAGS+=-Wl,--section-start=.text=$(CONFIG_AVR_BLDADDR)
APPSIZE=-$(CONFIG_AVR_BLDADDR)
FBANNER=BOOT FLASH usage
else
FBANNER=FLASH usage
endif


ifeq ($(CONFIG_AVR_EXTRA_SIZE_OPT),y)
CFLAGS+= -mcall-prologues -fno-tree-scev-cprop -fno-split-wide-types -fpack-struct -fshort-enums
CONFIG_LDFLAGS=-Wl,--relax
endif

ifeq ($(CONFIG_AVR_VFPRINTF_MIN),y)
ELFFLAGS+= -Wl,-u,vfprintf -lprintf_min
endif

ifeq ($(CONFIG_AVR_VFPRINTF_FULL),y)
ELFFLAGS+= -Wl,-u,vfprintf -lprintf_flt -lm
endif

ifeq ($(CONFIG_AVR_VFSCANF_MIN),y)
ELFFLAGS+= -Wl,-u,vfscanf -lscanf_min
endif

ifeq ($(CONFIG_AVR_VFSCANF_FULL),y)
ELFFLAGS+= -Wl,-u,vfscanf -lscanf_flt -lm
endif


ifeq ($(CONFIG_AVR_OLD_DELAY),y)
	CFLAGS+=-D__DELAY_BACKWARD_COMPATIBLE__
endif


HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature
HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings

FLASHSIZE= $(shell echo $$((`echo -e "\#include <avr/io.h>\nFLASHEND" | avr-cpp -mmcu=$(MCU) | sed '$$!d'` + 1 $(APPSIZE) )))
RAMSTART= $(shell echo $$((`echo -e "\#include <avr/io.h>\nRAMSTART" | avr-cpp -mmcu=$(MCU) | sed '$$!d'`)))
RAMEND=$(shell echo $$((`echo -e "\#include <avr/io.h>\nRAMEND" | avr-cpp -mmcu=$(MCU) | sed '$$!d'` )))
RAMSIZE=$(shell echo $$(($(RAMEND)-$(RAMSTART) + 1 )))
EESIZE= $(shell echo $$((`echo -e "\#include <avr/io.h>\nE2END" | avr-cpp -mmcu=$(MCU) | sed '$$!d'` + 1 )))



%.hex: %.elf
		$(SILENT_OBJCOPY) $(OBJCOPY) -O ihex $(HEX_FLASH_FLAGS)  $< $@	

%.eep: %.elf
		$(SILENT_OBJCOPY) $(OBJCOPY) $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.eep.bin: %.elf
		$(SILENT_OBJCOPY) $(OBJCOPY) $(HEX_EEPROM_FLAGS) -O binary $< $@ || exit 0


sizecheck: $(filter-out sizecheck,$(BUILDGOALS))
	$(Q)$(ANTARES_DIR)/scripts/meter "$(FBANNER)" \
	`$(SIZE) $(IMAGENAME).elf |grep elf|awk '{print $$1+$$2}'` \
	$(FLASHSIZE);
	$(Q)$(ANTARES_DIR)/scripts/meter "EEPROM Usage" \
	`$(STAT) $(IMAGENAME).eep.bin -c %s` \
	$(EESIZE);
	$(Q)$(ANTARES_DIR)/scripts/meter "RAM Usage" \
	`$(SIZE) $(IMAGENAME).elf |grep elf|awk '{print $$2+$$3}'` \
	$(RAMSIZE);
	$(Q)echo "Note: Ram usage is only rough minimum estimation (.data + .bss)"

list-interrupts:
	echo "#include <avr/interrupt.h>" | $(TOOL_PREFIX)gcc -mmcu=$(MCU) -dM -E -|grep "_vect" | grep -v "_num" | awk '{printf $$2"\n"}'


BUILDGOALS+=sizecheck
PHONY+=sizecheck
