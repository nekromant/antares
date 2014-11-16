ARCH_FEATURES:=

FW_FILE_1	= 0x00000
FW_FILE_2	= 0x09000
#Set our build goals
BUILDGOALS=$(IMAGENAME).elf $(IMAGENAME)-$(FW_FILE_1).bin $(IMAGENAME)-$(FW_FILE_2).bin

# Do not combine objects into built-in.o
# Failing to do so screws up things and breaks ANTARES_* macros
# So it's 100% safe to set this to y
LD_NO_COMBINE=y

CFLAGS	  += -Wpointer-arith -Wundef -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals -D__ets__ -DICACHE_FLASH

CFLAGS	  += -I$(ANTARES_INSTALL_DIR)/src/arch/esp8266/include-libc
CFLAGS	  += -I$(ANTARES_INSTALL_DIR)/src/arch/esp8266/include-sdk

GCC_LDFILE = $(ANTARES_INSTALL_DIR)/src/arch/esp8266/ld/eagle.app.v6.ld

ELFFLAGS  +=  -L$(ANTARES_INSTALL_DIR)/src/arch/esp8266/lib

#Link our binary blobbies
LIBS	   =          c gcc hal phy net80211 lwip wpa main
ELFFLAGS  +=  -Wl,--start-group $(addprefix -l,$(LIBS)) -lgcc -Wl,--end-group

COMMON_LDFLAGS = -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static
LDFLAGS	  +=  $(COMMON_LDFLAGS)
ELFFLAGS  += $(COMMON_LDFLAGS) 


#FW_FILE_1_ARGS	= -bo $@ -bs .text -bs .data -bs .rodata -bc -ec
#FW_FILE_2_ARGS	= -es .irom0.text $@ -ec

FW_FILE_1_ARGS	= -bo $@ -bs .text -bs .data -bs .rodata -bc -ec
FW_FILE_2_ARGS	= -es .irom0.text $@ -ec

ifeq ($(CONFIG_ESP8266_FORCE_IROM),y)
before-link+=_move_code_to_irom
_move_code_to_irom: builtin 
	echo "`$(ANTARES_DIR)/scripts/parseobjs $(TOPDIR)/build/built-in.o` \
	`$(ANTARES_DIR)/scripts/parseobjs $(TOPDIR)/build/app/built-in.o`" | while read file; do \
	$(OBJCOPY) --rename-section .text=.irom0.text \
		--rename-section .literal=.irom0.literal $$file; \
	done
endif


$(IMAGENAME)-$(FW_FILE_1).bin: $(IMAGENAME).elf
	esptool -eo $< $(FW_FILE_1_ARGS)  

$(IMAGENAME)-$(FW_FILE_2).bin: $(IMAGENAME).elf
	esptool -eo $< $(FW_FILE_2_ARGS) 

PHONY+=_move_code_to_irom