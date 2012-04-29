objects-y+=usbdrv.o usbdrvasm.o oddebug.o

#vusb needs some params: 
CFLAGS+=-DUSB_CFG_IOPORTNAME=$(call unquote,$(CONFIG_USB_CFG_IOPORTNAME))
CFLAGS+=-DUSB_CFG_PULLUP_IOPORTNAME=$(call unquote,$(CONFIG_USB_CFG_PULLUP_IOPORTNAME))


U_VID=$(shell $(SRCDIR)/scripts/avr/vusb_id $(CONFIG_USB_CFG_VENDOR_ID))
U_PID=$(shell $(SRCDIR)/scripts/avr/vusb_id $(CONFIG_USB_CFG_DEVICE_ID))
U_VER=$(shell $(SRCDIR)/scripts/avr/vusb_id $(CONFIG_USB_CFG_DEVICE_VERSION))


#$(eval $(shell $(SRCDIR)/scripts/avr/vusb_str USB_CFG_VENDOR_NAME $(CONFIG_USB_CFG_VENDOR_NAME) str ))
#$(eval $(shell $(SRCDIR)/scripts/avr/vusb_str USB_CFG_VENDOR_NAME $(CONFIG_USB_CFG_VENDOR_NAME) len ))

$(info ==> $(CFG_LEN))

CFLAGS+= -DUSB_CFG_VENDOR_ID=$(U_VID) 
CFLAGS+= -DUSB_CFG_DEVICE_ID=$(U_PID)
CFLAGS+= -DUSB_CFG_DEVICE_VERSION=$(U_VER)

define vusb_def_string

$$(eval $$(shell $(SRCDIR)/scripts/avr/vusb_str $(1) $$(CONFIG_$(1)) str ))
$$(eval $$(shell $(SRCDIR)/scripts/avr/vusb_str $(1) $$(CONFIG_$(1)) len ))

CFLAGS+= "-D$(1)=$$($(1))"
CFLAGS+= -D$(1)_LEN=$$($(1)_LEN)        
endef


$(eval $(call vusb_def_string,USB_CFG_VENDOR_NAME))
$(eval $(call vusb_def_string,USB_CFG_DEVICE_NAME))
$(eval $(call vusb_def_string,USB_CFG_SERIAL_NUMBER))
