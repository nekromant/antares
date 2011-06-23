#define a help message
deploy_help+="jetterm - Run jetterm after flash"
#the deployment action

jetterm: avrdude
	stty -F $(call unquote,$(CONFIG_DEPLOY_JETTERM_PORT)) $(call unquote,$(CONFIG_DEPLOY_JETTERM_BAUD))
	cat $(call unquote,$(CONFIG_DEPLOY_JETTERM_PORT))| ./jetterm
	
