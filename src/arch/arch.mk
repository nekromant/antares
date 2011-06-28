ifeq ($(CONFIG_ARCH_AVR),y)
  ARCH=avr
endif
ifeq ($(CONFIG_ARCH_MSP430),y)
  ARCH=msp430
endif
ifeq ($(CONFIG_ARCH_8051),y)
  ARCH=8051
endif

ARCH?=avr
export ARCH

include src/arch/$(ARCH)/make.mk

archtest:
	echo "ARCH IS $(ARCH)"
