#define a help message
deploy_help+="\nstcdude - Flash the firmware via stcdude"
#the deployment action

root=$(call check_root,$(CONFIG_DEPLOY_ROOT))


stcdude:
	$(root) stcdude -p $(CONFIG_DEPLOY_STCDUDE_PORT) \
	 -b $(CONFIG_DEPLOY_STCDUDE_BAUDLOW):$(CONFIG_DEPLOY_STCDUDE_BAUDHIGH) \
	 $(call unquote,$(CONFIG_DEPLOY_STCDUDE_COMMANDLINE)) -a wflash -f $(IMAGENAME).bin


