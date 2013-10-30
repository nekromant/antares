include $(ANTARES_DIR)/src/arch/msp430/mcu_database.mk

ARCH_FEATURES:=ANTARES_STARTUP


#Set our build goals
BUILDGOALS=$(IMAGENAME).bin $(IMAGENAME).lss $(IMAGENAME).elf

# Do not combine objects into built-in.o
# Failing to do so screws up things and breaks ANTARES_* macros
# So it's 100% safe to set this to y
LD_NO_COMBINE=y

TARGET_ELFFILE=$(IMAGENAME).elf
TARGET_BINFILE=$(IMAGENAME).bin
TARGET_EEPFILE=$(IMAGENAME).eep

CFLAGS+= -mmcu=$(MCU) -DF_CPU=$(CONFIG_F_CPU)
ELFFLAGS+= -mmcu=$(MCU)

%.hex: %.elf
		$(SILENT_OBJCOPY) $(OBJCOPY) -O ihex $(HEX_FLASH_FLAGS)  $< $@	
		$(call colorize,$(t_cyn))
		@echo "Created Intel HEX file: $(@)"
		$(call colorize,$(col_rst))

list-interrupts: 
	$(Q)for p in `echo | $(CPP) -v 2>&1|grep "/include"`; do \
	 if [ -f $$p/$(MCU).h ]; then \
	  cat $$p/$(MCU).h|grep VECTOR | \
		awk '{printf $$2"\t"; $$1=$$2=$$3="";print $$0}'; \
	 fi \
	done	

#HACK!
include $(ANTARES_DIR)/deploy/mspdebug/drv_selection.mk
root=$(call check_root,$(CONFIG_DEPLOY_ROOT))

probe:
ifeq ($(CONFIG_DEPLOY_MSPDEBUG),y)
	$(Q)cp $(TOPDIR)/.config $(TOPDIR)/.config.probe
	$(Q)$(root) mspdebug $(MSPDEBUGDRV) exit > $(TMPDIR)/probedata
	$(ANTARES_DIR)/scripts/msp430probe2config $(TMPDIR)/probedata > $(TMPDIR)/probedata.config
	$(Q)cat $(TMPDIR)/probedata
	$(Q)echo "#Autodected the following config"
	$(Q)cat $(TMPDIR)/probedata.config
	$(Q)echo
	$(Q)echo "Does this config look good to you? [y/n]"
	$(Q)read -N1 c > /dev/null;[ "$c"=="y" ] && cat $(TMPDIR)/probedata.config >> $(TOPDIR)/.config
	$(Q)echo " you said, so be the configuration updated"
	$(Q)echo "backup saved as .config.probe"
else
	$(Q)echo "FATAL: probing requires mspdebug deployment"
endif


sizecheck: $(filter-out sizecheck,$(BUILDGOALS))
	$(Q)echo -e "FLASH usage:\t" \
	`$(SIZE) $(IMAGENAME).elf |grep elf|awk '{print $$1+$$2}'` bytes 
	$(Q)echo -e "RAM usage:\t" \
	`$(SIZE) $(IMAGENAME).elf |grep elf|awk '{print $$2+$$3}'` bytes

BUILDGOALS+=sizecheck
PHONY+=list-interrupts probe sizecheck
