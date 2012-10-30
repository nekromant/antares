#define a help message
deploy_help+="\nzmodem - upload firmware via zmodem"
#the deployment action

root=$(call check_root,$(CONFIG_DEPLOY_ROOT))

STR=$(call unquote,$(CONFIG_DEPLOY_ZMODEM_STR))
zmodem:
	$(root) stty -F $(CONFIG_DEPLOY_ZMODEM_PORT) -crtscts -cstopb -parenb clocal cread 
ifeq ($(CONFIG_DEPLOY_ZMODEM_ISTR),y)
	$(root) sh -c 'echo -ne "$(STR)" > $(CONFIG_DEPLOY_ZMODEM_PORT)'
endif
	$(root) sh -c 'sz -a $(IMAGENAME).bin > $(CONFIG_DEPLOY_ZMODEM_PORT) < $(CONFIG_DEPLOY_ZMODEM_PORT)'


