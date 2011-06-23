#define a help message
deploy_help+="avrdude - Flash the firmware via avrdude"
#the deployment action

avrdude:
	avrdude $(call unquote,$(CONFIG_DEPLOY_AVRDUDE_COMMANDLINE)) -U flash:w:$(TARGET_HEXFILE)
	
