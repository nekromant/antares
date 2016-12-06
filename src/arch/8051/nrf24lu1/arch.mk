CONFIG_IRAM_SIZE=256
CONFIG_XRAM_SIZE=2048
CONFIG_FLASH_SIZE=16384
COMMONFLAGS+=--xram-loc 0x8000 

CFLAGS+=-DCONFIG_FLASH_SIZE=$(CONFIG_FLASH_SIZE)