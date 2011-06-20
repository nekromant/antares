#add any new method right here

ifeq ($(CONFIG_DEPLOY_AVRDUDE),y)
	include deploy/avrdude/Makefile
endif


ifeq ($(CONFIG_DEPLOY_AVRDUDE_REMOTE),y)
	include deploy/avrdude-remote/Makefile
endif
