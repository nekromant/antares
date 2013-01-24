include $(TOPDIR)/.config
include $(ANTARES_DIR)/src/arch/arm/stm32/startup/Makefile
include $(ANTARES_DIR)/make/Makefile.lib

STARTUP_FILE=$(patsubst %.o,%.s,$(objects-y))
STARTUP=$(ANTARES_DIR)/src/arch/arm/stm32/startup/$(STARTUP_FILE)

list-interrupts:
	@echo "Startup file is $(STARTUP_FILE)"
	@cat $(STARTUP)|grep "\.weak"|awk '{print $$2}'

root=$(call check_root,$(CONFIG_DEPLOY_ROOT))
stm32f=$(call unquote, $(CONFIG_DEPLOY_STM32FLASH_BIN))

stm32probe:
ifeq ($(CONFIG_DEPLOY_STM32FLASH),y)
	@cp $(TOPDIR)/.config $(TOPDIR)/.config.stm32probe
	@$(root) $(stm32f) $(CONFIG_DEPLOY_STM32FLASH_PORT) > $(TMPDIR)/probedata
	@cat $(TMPDIR)/probedata | $(ANTARES_DIR)/scripts/stm32flash2config > $(TMPDIR)/probedata.config
	@cat $(TMPDIR)/probedata
	@cat $(TMPDIR)/probedata.config
	@echo "Does this config look good to you? [y/n]"
	@read -N1 c > /dev/null;[ "$c"=="y" ] && cat $(TMPDIR)/probedata.config >> $(TOPDIR)/.config
	@echo " you said, so be the configuration updated"
	@echo "backup saved as .config.stm32probe"
else
	@echo "FATAL: please enable stm32flash deployment "
endif
