include deploy/mspdebug/drv_selection.mk
deploy_help+="\nmspdebug - deploy using mspdebug"

root=$(call check_root,$(CONFIG_DEPLOY_ROOT))

mspdebug: 
	$(root) mspdebug $(MSPDEBUGDRV) "load $(TARGET_ELFFILE)"
