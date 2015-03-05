ifeq ($(CONFIG_LIB_RF24_HAVE_AVRSPI),y)
cflags-y+=-DCSN_PORT=PORT$(call unquote,$(CONFIG_LIB_RF24_CSN_PORT))
cflags-y+=-DCE_PORT=PORT$(call unquote,$(CONFIG_LIB_RF24_CE_PORT))
cflags-y+=-DCSN_DDR=DDR$(call unquote,$(CONFIG_LIB_RF24_CSN_PORT))
cflags-y+=-DCE_DDR=DDR$(call unquote,$(CONFIG_LIB_RF24_CE_PORT))

cflags-y+=-DSPI_PORTX=PORT$(call unquote,$(CONFIG_LIB_RF24_SPI_PORT))
cflags-y+=-DSPI_DDRX=DDR$(call unquote,$(CONFIG_LIB_RF24_SPI_PORT))
cflags-y+=-DSPI_PINX=PIN$(call unquote,$(CONFIG_LIB_RF24_SPI_PORT))


cflags-y+=-DCE_PORT=PORT$(call unquote,$(CONFIG_LIB_RF24_CE_PORT))
cflags-y+=-DCSN_DDR=DDR$(call unquote,$(CONFIG_LIB_RF24_CSN_PORT))
cflags-y+=-DCE_DDR=DDR$(call unquote,$(CONFIG_LIB_RF24_CE_PORT))
endif

objects-$(CONFIG_LIB_RF24_AVR_SOFTSPI)+=bindings-avr-soft.o
objects-$(CONFIG_LIB_RF24_AVR_HARDSPI)+=bindings-avr-hard.o

objects-$(CONFIG_LIB_RF24)+=rf24.o
objects-$(CONFIG_LIB_RF24_SWEEP)+=rf24-sweep.o

