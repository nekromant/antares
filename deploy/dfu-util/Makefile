deploy_help+="\ndfu-util - flash via dfu utility"

root=$(call check_root,$(CONFIG_DEPLOY_ROOT))

dfu-util: 
	$(root) dfu-util --alt 1 --intf 0 -D $(IMAGENAME).bin
