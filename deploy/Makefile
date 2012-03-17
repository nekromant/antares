#add any new method right here
#TODO: Autogen this sh*t

ifeq ($(CONFIG_DEPLOY_AVRDUDE),y)
	include $(SRCDIR)/deploy/avrdude/Makefile
endif

ifeq ($(CONFIG_DEPLOY_JETTERM),y)
        include $(SRCDIR)/deploy/jetterm/Makefile
endif

ifeq ($(CONFIG_DEPLOY_STM32FLASH),y)
        include $(SRCDIR)/deploy/stm32flash/Makefile
endif


ifeq ($(CONFIG_DEPLOY_AVRDUDE_REMOTE),y)
	include $(SRCDIR)/deploy/avrdude-remote/Makefile
endif

ifeq ($(CONFIG_DEPLOY_MSPDEBUG),y)
        include $(SRCDIR)/deploy/mspdebug/Makefile
endif

ifeq ($(CONFIG_DEPLOY_OPENOCD),y)
	include $(SRCDIR)/deploy/openocd/Makefile
endif