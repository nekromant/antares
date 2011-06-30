#define a help message
deploy_help+="minicom - Run minicom after flash"
#the deployment action

minicom: avrdude
	minicom -o -D $(call unquote,$(CONFIG_DEPLOY_JETTERM_PORT)) -b $(call unquote,$(CONFIG_DEPLOY_JETTERM_BAUD))
	
