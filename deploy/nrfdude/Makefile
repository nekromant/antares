#define a help message
deploy_help+="\nrun - run the binary"
#the deployment action

root=$(call check_root,$(CONFIG_DEPLOY_ROOT))

nrfdude:
	$(root) nrfdude --ihx --wflash $(IMAGENAME).ihx

