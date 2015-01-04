#define a help message
deploy_help+="\nesptool - Flash the firmware via esptool"

#the deployment action

#GP0 - Loader GPIO
#GP1 - reset
#Press both.
#release reset.
#sleep 300 ms
#release boot
define tobootloader
	pl2303gpio --gpio=0 --out=0 --gpio=1 --out=0 \
	--gpio=1 --in --sleep 50 \
	--gpio=0 --in
endef

define reset
	pl2303gpio --gpio=1 --out=0 --gpio=1 --in
endef

root=$(call check_root,$(CONFIG_DEPLOY_ROOT))
esptool=$(call unquote, $(CONFIG_DEPLOY_ESPTOOL_BIN))

esptool:
ifeq ($(CONFIG_DEPLOY_ESPTOOL_PL2303),y)
	$(root) $(tobootloader)
endif

	$(root) $(esptool) --port $(CONFIG_DEPLOY_ESPTOOL_PORT) write_flash $(CONFIG_DEPLOY_ESPTOOL_ADDRESS) $(IMAGENAME).rom
ifeq ($(CONFIG_DEPLOY_ESPTOOL_PL2303),y)
	$(root) $(reset)
endif

ifeq ($(CONFIG_DEPLOY_ESPTOOL_MINICOM),y)
	$(root) minicom -o -D $(CONFIG_DEPLOY_ESPTOOL_PORT) -b 115200
endif
