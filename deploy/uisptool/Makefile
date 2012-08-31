#define a help message
deploy_help+="\nuisptool - Flash the firmware via uisptool"
#the deployment action

root=$(call check_root,$(CONFIG_DEPLOY_ROOT))

ifeq ($(CONFIG_DEPLOY_UISPTOOL_RUN),y)
RUN=-r
endif

ifeq ($(CONFIG_DEPLOY_UISPTOOL_NAME),)
CONFIG_DEPLOY_UISPTOOL_NAME:="unknown"
endif

ifeq ($(CONFIG_DEPLOY_UISPTOOL_VER),)
CONFIG_DEPLOY_UISPTOOL_VER:="unknown"
endif

uisptool:
	$(root) uisptool -u $(CONFIG_DEPLOY_UISPTOOL_NAME) $(CONFIG_DEPLOY_UISPTOOL_VER) -f $(TARGET_HEXFILE) $(RUN)

	
