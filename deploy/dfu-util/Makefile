deploy_help+="\ndfu-util - flash via dfu utility"

root=$(call check_root,$(CONFIG_DEPLOY_DFU_ROOT))

dfu-util: 
	$(root) dfu-util "load $(IMAGENAME).bin"