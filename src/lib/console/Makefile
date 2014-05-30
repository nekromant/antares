#Core stuff
objects-$(CONFIG_LIB_EARLYCON)+=earlyprintk.o

#Earlycon drivers
objects-$(CONFIG_LIB_EARLYCON_AVRSERIAL)+=earlycon-avrserial.o
objects-$(CONFIG_LIB_EARLYCON_VUSBLOWSERIAL)+=earlycon-vusb-lowserial.o

objects-$(CONFIG_LIB_EARLYCON_STCUART)+=earlycon-stc.o
objects-$(CONFIG_LIB_EARLYCON_NATIVE)+=earlycon-host.o
objects-$(CONFIG_LIB_EARLYCON_STLINKY)+=earlycon-stlinky.o
objects-$(CONFIG_LIB_EARLYCON_MSP430SOFTSERIAL)+=earlycon-msp430softserial.o
objects-$(CONFIG_LIB_EARLYCON_STM32F10XSERIAL)+=earlycon-stm32f10xserial.o
objects-$(CONFIG_LIB_EARLYCON_STM32F4XXSERIAL)+=earlycon-stm32f4xxserial.o

objects-$(CONFIG_LIB_EARLYCON_AVRSOFTSERIAL)+=earlycon-avrsoftserial.o
cflags-$(CONFIG_LIB_EARLYCON_AVRSOFTSERIAL)+=\
-DLIB_EARLYCON_AVR_SOFTSERIAL_PORT=$(call unquote,$(CONFIG_LIB_EARLYCON_AVR_SOFTSERIAL_PORT))


#Libc/printk glue
objects-$(CONFIG_LIB_EARLYCON_GLUE_AVR)+=glue-avrlibc.o
objects-$(CONFIG_LIB_EARLYCON_GLUE_SDCC)+=glue-sdcc.o
objects-$(CONFIG_LIB_EARLYCON_GLUE_MSP430)+=glue-msp430.o
objects-$(CONFIG_LIB_EARLYCON_GLUE_NEWLIB)+=glue-newlib.o



