#add any new method right here

ifeq ($(CONFIG_DEPLOY_AVRDUDE),y)
	include deploy/avrdude/Makefile
endif

ifeq ($(CONFIG_DEPLOY_JETTERM),y)
        include deploy/jetterm/Makefile
endif


ifeq ($(CONFIG_DEPLOY_AVRDUDE_REMOTE),y)
	include deploy/avrdude-remote/Makefile
endif

ifeq ($(CONFIG_DEPLOY_MINICOM),y)
        include deploy/minicom/Makefile
endif

