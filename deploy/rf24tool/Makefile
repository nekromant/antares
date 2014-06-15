#define a help message
deploy_help+="\nrf24tool - Send the firmware over the rf24tool wireless"
#the deployment action


ifeq ($(CONFIG_DEPLOY_RF24TOOL_RUN),y)
RUN+=--run
endif

ifneq ($(CONFIG_DEPLOY_RF24TOOL_VFY),y)
OP+=--noverify
endif


rf24tool:
	$(root) rf24tool --channel=$(CONFIG_DEPLOY_RF24TOOL_RF_CHANNEL) \
	--remote-addr=\
$(CONFIG_DEPLOY_RF24TOOL_RF_ADDR_0):\
$(CONFIG_DEPLOY_RF24TOOL_RF_ADDR_1):\
$(CONFIG_DEPLOY_RF24TOOL_RF_ADDR_2):\
$(CONFIG_DEPLOY_RF24TOOL_RF_ADDR_3):\
$(CONFIG_DEPLOY_RF24TOOL_RF_ADDR_4) \
	--part=$(CONFIG_DEPLOY_RF24TOOL_PART) --write --file=$(TARGET_BINFILE) $(OP) $(RUN)