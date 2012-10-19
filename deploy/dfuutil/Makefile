deploy_help+="\ndfuutil - flash via dfu utility"

root=$(call check_root,$(CONFIG_DEPLOY_ROOT))

dfuutil: 
	$(root) dfu-util --alt 1 --intf 0 -D $(IMAGENAME).bin
