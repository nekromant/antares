#define a help message
deploy_help+="\nrun - run the binary"
#the deployment action

root=$(call check_root,$(CONFIG_DEPLOY_ROOT))

run:
	$(root) $(IMAGENAME).elf $(call unquote,$(CONFIG_DEPLOY_RUN_ARGS))

