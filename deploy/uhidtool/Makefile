#define a help message
deploy_help+="\nuisptool - Flash the firmware via uisptool"
#the deployment action

root=$(call check_root,$(CONFIG_DEPLOY_ROOT))


uhidtool:
	$(root) uhidtool --part flash --write $(TARGET_BINFILE)
ifeq ($(CONFIG_DEPLOY_UHIDTOOL_RUN),y)
	$(root) uhidtool --run
endif

	
