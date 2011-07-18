#define a help message
deploy_help+="\navrdude - Flash the firmware via avrdude"
#the deployment action

avrdude:
	avrdude $(call unquote,$(CONFIG_DEPLOY_AVRDUDE_COMMANDLINE)) -U flash:w:$(TARGET_HEXFILE)
script:
	avrdude $(call unquote,$(CONFIG_DEPLOY_AVRDUDE_COMMANDLINE)) -U eeprom:w:./script.eep:r
	minicom -o -D /dev/ttyUSB0 -b 115200
	
