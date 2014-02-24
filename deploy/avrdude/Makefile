#define a help message
deploy_help+="\navrdude - Flash the firmware via avrdude"
deploy_help+="\n Use WFUSES=y make deploy-avrdude to write fuses"
#the deployment action

include $(SRCDIR)/deploy/avrdude/proglist.mk

root=$(call check_root,$(CONFIG_DEPLOY_ROOT))

ifeq ($(CONFIG_DEPLOY_AVRDUDE_OVR),y)
MCU:=$(call unquote,$(CONFIG_DEPLOY_AVRDUDE_PART))
endif

# Yes, freaky people use avrdude for pic32
# We even ship an avrdude.conf for those
ifeq ($(CONFIG_ARCH_PIC32),y)
	AVRDUDE_EXTRA=-C $(ANTARES_INSTALL_DIR)/extra/avrdude.pic32.conf
endif

RESETCMD:=
ifeq ($(CONFIG_DEPLOY_AVRDUDE_RESET),y)
	RESETCMD:= $(Q)$(root) $(ANTARES_INSTALL_DIR)/scripts/serial_reset.sh \
		$(CONFIG_DEPLOY_AVRDUDE_PORT)
endif

ifeq ($(CONFIG_DEPLOY_AVRDUDE_SER),y)
	AVRDUDE_EXTRA+=-P $(CONFIG_DEPLOY_AVRDUDE_PORT) 
	AVRDUDE_EXTRA+=-b $(CONFIG_DEPLOY_AVRDUDE_BAUDRATE)
endif

FUSES:=

ifeq ($(CONFIG_DEPLOY_AVRDUDE_FSLOW),y)
	FUSES+=-B 35
endif

ifeq ($(CONFIG_DEPLOY_AVRDUDE_FL),y)
	FUSES+=-U lfuse:w:$(CONFIG_DEPLOY_AVRDUDE_FLV):m
endif

ifeq ($(CONFIG_DEPLOY_AVRDUDE_FH),y)
	FUSES+=-U hfuse:w:$(CONFIG_DEPLOY_AVRDUDE_FHV):m
endif

ifeq ($(CONFIG_DEPLOY_AVRDUDE_FE),y)
	FUSES+=-U efuse:w:$(CONFIG_DEPLOY_AVRDUDE_FEV):m
endif


avrdude:
	$(RESETCMD)
ifeq ($(WFUSES),y)
	$(root) avrdude $(AVRDUDE_EXTRA) -c $(PROG) -p $(MCU) $(call unquote,$(CONFIG_DEPLOY_AVRDUDE_COMMANDLINE)) $(FUSES)
endif
	$(root) avrdude $(AVRDUDE_EXTRA) -c $(PROG) -p $(MCU) $(call unquote,$(CONFIG_DEPLOY_AVRDUDE_COMMANDLINE)) -U flash:w:$(TARGET_HEXFILE)

