#Core stuff
objects-$(CONFIG_LIB_EARLYCON)+=earlyprintk.o

#Earlycon drivers
objects-$(CONFIG_LIB_EARLYCON_AVRSERIAL)+=earlycon-avrserial.o
objects-$(CONFIG_LIB_EARLYCON_STCUART)+=earlycon-stc.o
objects-$(CONFIG_LIB_EARLYCON_NATIVE)+=earlycon-host.o
objects-$(CONFIG_LIB_EARLYCON_STLINKY)+=earlycon-stlinky.o

#Libc/printk glue
objects-$(CONFIG_ARCH_AVR)+=glue-avrlibc.o
objects-$(CONFIG_ARCH_8051)+=glue-sdcc.o
objects-$(CONFIG_ARCH_ARM)+=glue-newlib.o



