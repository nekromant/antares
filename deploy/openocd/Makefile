include deploy/mspdebug/drv_selection.mk
deploy_help+="\nopenocd - deploy using openocd"

root=$(call check_root,$(CONFIG_DEPLOY_ROOT))

#TODO: Better kcnf interface
openocd: 
	$(root) openocd $(call unquote,$(DEPLOY_OPENOCD_COMMANDLINE))
