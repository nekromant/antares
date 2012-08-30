deploy_help+="\nstm32flash - Flash the firmware via stm32flash"

ifeq ($(CONFIG_DEPLOY_STM32FLASH_DFWP),y)
stm32f_extraflags+=-u
endif

ifeq ($(CONFIG_DEPLOY_STM32FLASH_FBP),y)
stm32f_extraflags+=-f
endif

ifeq ($(CONFIG_DEPLOY_STM32FLASH_VFY),y)
stm32f_extraflags+=-v
endif


stm32f_extraflags+= -n $(CONFIG_DEPLOY_STM32FLASH_RETRY_COUNT)

ifeq ($(CONFIG_DEPLOY_STM32FLASH_EXEC),y)
stm32f_extraflags+=-g $(CONFIG_DEPLOY_STM32FLASH_EXEC_ADDR)
endif

root=$(call check_root,$(CONFIG_DEPLOY_ROOT))
stm32f=$(call unquote, $(CONFIG_DEPLOY_STM32FLASH_BIN))



stm32flash:
	$(root) $(stm32f) -w $(IMAGENAME).bin $(stm32f_extraflags) $(CONFIG_DEPLOY_STM32FLASH_PORT) -b $(CONFIG_DEPLOY_STM32FLASH_BAUD)
