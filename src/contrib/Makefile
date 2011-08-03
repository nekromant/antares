objects-$(CONFIG_CONTRIB_CRUMBO)+=crumbo/
objects-$(CONFIG_AVRLIB)+=avrlib/
objects-$(CONFIG_CONTRIB_VUSB)+=usbdrv/ usbdrv.gen
#vusb needs some params: 
cflags-$(CONFIG_CONTRIB_VUSB)+=-DUSB_CFG_IOPORTNAME=$(call unquote,$(CONFIG_USB_CFG_IOPORTNAME))
cflags-$(CONFIG_USB_OPT)+=-DUSB_CFG_PULLUP_IOPORTNAME=$(call unquote,$(CONFIG_USB_CFG_PULLUP_IOPORTNAME))


U_VID=$(shell $(TOP_DIR)/scripts/avr/vusb_id $(CONFIG_USB_CFG_VENDOR_ID))
U_PID=$(shell $(TOP_DIR)/scripts/avr/vusb_id $(CONFIG_USB_CFG_DEVICE_ID))
U_VER=$(shell $(TOP_DIR)/scripts/avr/vusb_id $(CONFIG_USB_CFG_DEVICE_VERSION))
$(eval $(shell $(TOP_DIR)/scripts/avr/vusb_str CFG))
$(info ==> $(CFG_LEN))

cflags-$(CONFIG_CONTRIB_VUSB)+=-DUSB_CFG_VENDOR_ID=$(U_VID) 
cflags-$(CONFIG_CONTRIB_VUSB)+=-DUSB_CFG_DEVICE_ID=$(U_PID)
cflags-$(CONFIG_CONTRIB_VUSB)+=-DUSB_CFG_DEVICE_VERSION=$(U_VER)  

