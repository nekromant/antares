#This a database, that sets all the different params of AVR
#MCUs that will be later be utilised while building and deploying

ifeq ($(CONFIG_MCU_M8),y)
  MCU=atmega8
  FLASH_SIZE=8192
  RAM_SIZE=4096
  EEPROM_SIZE=1024
endif

ifeq ($(CONFIG_MCU_M128),y)
  MCU=atmega128
  FLASH_SIZE=131072
  RAM_SIZE=4096
  EEPROM_SIZE=1024
endif


ifeq ($(CONFIG_MCU_M1280),y)
  #TODO: Check teh values
  MCU=atmega1280
  FLASH_SIZE=131072
  RAM_SIZE=4096
  EEPROM_SIZE=1024 
endif

#MCU?=atmega8

export MCU FLASH_SIZE RAM_SIZE EEPROM_SIZE
