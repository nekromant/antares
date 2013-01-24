include $(TOPDIR)/.config
include $(ANTARES_DIR)/src/arch/arm/stm32/startup/Makefile
STARTUP_FILE=$(patsubst %.o,%.s,$(objects-y))
STARTUP=$(ANTARES_DIR)/src/arch/arm/stm32/startup/$(STARTUP_FILE)

list-interrupts:
	@echo "Startup file is $(STARTUP_FILE)"
	@cat $(STARTUP)|grep "\.weak"|awk '{print $$2}'
