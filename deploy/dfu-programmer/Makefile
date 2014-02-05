deploy_help+="\ndfuprog - deploy using dfu-programmer"
include $(ANTARES_DIR)/deploy/dfu-programmer/proglist.mk

root=$(call check_root,$(CONFIG_DEPLOY_ROOT))

dfuprog:
ifeq ($(CONFIG_ARCH_AVR), y)
	$(root) dfu-programmer $(DEPLOY_TARGET) flash $(IMAGENAME).hex
else
	$(root) dfu-programmer $(DEPLOY_TARGET) flash $(IMAGENAME).ihx
endif
