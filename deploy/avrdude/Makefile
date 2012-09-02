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


avrdude:
	$(root) avrdude -c $(PROG) -p $(MCU) $(call unquote,$(CONFIG_DEPLOY_AVRDUDE_COMMANDLINE)) -U flash:w:$(TARGET_HEXFILE)


