deploy_help+="\ndfuprog - deploy using dfu-programmer"
include $(ANTARES_DIR)/deploy/dfu-programmer/proglist.mk

root=$(call check_root,$(CONFIG_DEPLOY_ROOT))

fw_erase=

ifeq ($(CONFIG_ARCH_AVR), y)
img_ext=hex
fw_erase=$(root) dfu-programmer $(DEPLOY_TARGET) erase
else
img_ext=ihx
endif

fw_run=
ifeq ($(CONFIG_DEPLOY_DFUPRG_RUN), y)
fw_run=$(root) dfu-programmer $(DEPLOY_TARGET) start
endif

dfuprog:
	$(fw_erase)
	$(root) dfu-programmer $(DEPLOY_TARGET) flash $(IMAGENAME).$(img_ext)
	$(fw_run)
