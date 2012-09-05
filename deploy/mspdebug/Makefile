include $(ANTARES_DIR)/deploy/mspdebug/drv_selection.mk
deploy_help+="\nmspdebug - deploy using mspdebug"

root=$(call check_root,$(CONFIG_DEPLOY_ROOT))

mspdebug: 
	 $(root) mspdebug $(MSPDEBUGDRV) "prog $(TARGET_ELFFILE)"
