ARCH_FEATURES:=

FW_FILE_1	= 0x00000
FW_FILE_2	= 0x0b000
#Set our build goals
BUILDGOALS=$(IMAGENAME).rom

# Do not combine objects into built-in.o
# Failing to do so screws up things and breaks ANTARES_* macros
# So it's 100% safe to set this to y
LD_NO_COMBINE=y

CFLAGS	  += -Wpointer-arith -Wundef -Wl,-EL -mtext-section-literals -fno-inline-functions -mlongcalls -D__ets__ -DICACHE_FLASH

CFLAGS	  += -I$(ANTARES_DIR)/src/arch/esp8266/include-sdk

GCC_LDFILE = $(ANTARES_DIR)/src/arch/esp8266/ld/eagle.app.v6.ld


#Link our binary blobbies

define esp_check_lib
ifeq ($$($(1)),y)
ELFFLAGS+=-l$(2)
endif
endef

ELFFLAGS  += -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static -Wl,--start-group 
ELFFLAGS  += -Wl,-Map=$(IMAGENAME).map

$(eval $(call esp_check_lib,CONFIG_ESP8266_BLOB_MAIN,main))
$(eval $(call esp_check_lib,CONFIG_ESP8266_BLOB_HAL,hal))
$(eval $(call esp_check_lib,CONFIG_ESP8266_BLOB_NET80211,net80211))
$(eval $(call esp_check_lib,CONFIG_ESP8266_BLOB_PHY,phy))
$(eval $(call esp_check_lib,CONFIG_ESP8266_BLOB_PP,pp))
$(eval $(call esp_check_lib,CONFIG_ESP8266_BLOB_WPA,wpa))
$(eval $(call esp_check_lib,CONFIG_ESP8266_BLOB_SSL,ssl))
$(eval $(call esp_check_lib,CONFIG_ESP8266_BLOB_LWIP,lwip))

ELFFLAGS  += -lm 

ifeq ($(CONFIG_ESP8266_LIBC_IROM),y)
ELFFLAGS  += -lcirom
else
ELFFLAGS  += -lc 
endif

ELFFLAGS  += -lgcc -Wl,--end-group

ifneq ($(CONFIG_ESP8266_SDK),)
CFLAGS    += -I$(CONFIG_ESP8266_SDK)/include
ELFFLAGS  += -L$(CONFIG_ESP8266_SDK)/lib
endif

LDFLAGS   += $(COMMON_LDFLAGS)
ELFFLAGS  += $(COMMON_LDFLAGS) 

ifneq ($(CONFIG_BUILD_VERBOSE),y)
   SILENT_ESPTOOL  = @echo '  $(tb_grn)[ESPTOOL]$(col_rst)  ' $(@);
endif

ifeq ($(CONFIG_ESP8266_FORCE_IROM),y)
before-link+=_move_code_to_irom

# Some really dark magic in bash. Feel teh power!
# In short. the simple way is rename .text=.irom0.text and .literal=.irom0.literal
# However if we use gc-sections, we end up with .text.symbolname and .literal.symbolname
# So we iterate around all sections that start with .text and carry out the dirty work
# Yep. Say thanks to Espressif for trying to enforce the weird ICACHE_FLASH_ATTR way.
# FixMe: We could theoretically parallelize the objcopy, but since it will require some
# hacking of toolchains/gcc.mk - let it be. 

_move_code_to_irom: builtin 
	@echo "  $(tb_red)[IROMIFY]$(col_rst)   Moving application code to $(tb_red)IROM$(col_rst)" 
	$(Q)echo -e "`$(ANTARES_DIR)/scripts/parseobjs $(TOPDIR)/build/built-in.o` \n \
	`$(ANTARES_DIR)/scripts/parseobjs $(TOPDIR)/build/app/built-in.o`" | while read file; do \
	for section in `$(OBJDUMP) -h $$file|grep text|grep -v irom|grep -v iram|awk '{print $$2}'`; do \
		sname=`echo $$section|sed 's/.text.//'`; 							\
		$(OBJCOPY) --rename-section .text.$$sname=.irom0.text.$$sname \
				--rename-section .literal.$$sname=.irom0.literal.$$sname $$file; \
	done; \
	$(OBJCOPY) --rename-section .text=.irom0.text \
		--rename-section .literal=.irom0.literal $$file; \
	done
endif

#TODO Set flash size via menuconfig
FLASH_SIZE=512*1024
FBANNER=Flash usage

$(IMAGENAME).rom: $(IMAGENAME).elf
	$(SILENT_ESPTOOL)esptool.py elf2image -o $(IMAGENAME)- $(IMAGENAME).elf
	$(Q)dd if=/dev/zero of=$(@) bs=1k count=512 2>/dev/null
	$(Q)dd if=$(IMAGENAME)-$(FW_FILE_1).bin of=$(@) conv=notrunc 2>/dev/null
	$(Q)dd if=$(IMAGENAME)-$(FW_FILE_2).bin of=$(@) bs=1 seek=$$(($(FW_FILE_2))) 2>/dev/null
	$(Q)$(ANTARES_DIR)/scripts/meter "$(FBANNER)" `wc -c<$(@)`  $(FLASH_SIZE)


PHONY+=_move_code_to_irom
