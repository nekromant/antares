#define a help message
deploy_help+="\navrdude - Flash the firmware via avrdude"
#the deployment action

include $(SRCDIR)/deploy/avrdude/proglist.mk

$(info $(CONFIG_AVRDUDE_PROG_USBASP))
$(info $(PROG))
root=$(call check_root,$(CONFIG_DEPLOY_ROOT))

ifeq ($(CONFIG_DEPLOY_AVRDUDE_OVR),y)
MCU:=$(call unquote,$(CONFIG_DEPLOY_AVRDUDE_PART))
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
	$(root) avrdude -c $(PROG) -p $(MCU) $(call unquote,$(CONFIG_DEPLOY_AVRDUDE_COMMANDLINE)) -U flash:w:$(TARGET_HEXFILE)


avrdude-fuses:
	$(root) avrdude -c $(PROG) -p $(MCU) $(call unquote,$(CONFIG_DEPLOY_AVRDUDE_COMMANDLINE)) $(FUSES)


