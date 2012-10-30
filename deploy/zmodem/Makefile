#define a help message
deploy_help+="\nzmodem - upload firmware via zmodem"
#the deployment action

root=$(call check_root,$(CONFIG_DEPLOY_ROOT))

PRE_STR=$(call unquote,$(CONFIG_DEPLOY_ZMODEM_PRESTR))
POST_STR=$(call unquote,$(CONFIG_DEPLOY_ZMODEM_POSTSTR))

zmodem:
	$(root) stty -F $(CONFIG_DEPLOY_ZMODEM_PORT) -crtscts -cstopb -parenb clocal cread 
ifneq ($(PRE_STR),)
	$(root) sh -c 'echo -ne "$(PRE_STR)" > $(CONFIG_DEPLOY_ZMODEM_PORT)'
	sleep 1
endif
	$(root) sh -c 'sz -a $(IMAGENAME).bin > $(CONFIG_DEPLOY_ZMODEM_PORT) < $(CONFIG_DEPLOY_ZMODEM_PORT)'
ifneq ($(POST_STR),)
	sleep 1
	$(root) sh -c 'echo -ne "$(POST_STR)" > $(CONFIG_DEPLOY_ZMODEM_PORT)'
endif


