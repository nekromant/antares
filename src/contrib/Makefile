objects-$(CONFIG_CONTRIB_CRUMBO)+=crumbo/
objects-$(CONFIG_AVRLIB)+=avrlib/
objects-$(CONFIG_CONTRIB_VUSB)+=usbdrv/ usbdrv.gen
#vusb needs some params: 
commonflags-$(CONFIG_CONTRIB_VUSB)+=-DUSB_CFG_IOPORTNAME=$(call unquote,$(CONFIG_USB_CFG_IOPORTNAME))
commonflags-$(CONFIG_USB_OPT)+=-DUSB_CFG_PULLUP_IOPORTNAME=$(call unquote,$(CONFIG_USB_CFG_PULLUP_IOPORTNAME))


U_VID=$(shell $(TOP_DIR)/scripts/avr/vusb_id $(CONFIG_USB_CFG_VENDOR_ID))
U_PID=$(shell $(TOP_DIR)/scripts/avr/vusb_id $(CONFIG_USB_CFG_DEVICE_ID))
U_VER=$(shell $(TOP_DIR)/scripts/avr/vusb_id $(CONFIG_USB_CFG_DEVICE_VERSION))


#$(eval $(shell $(TOP_DIR)/scripts/avr/vusb_str USB_CFG_VENDOR_NAME $(CONFIG_USB_CFG_VENDOR_NAME) str ))
#$(eval $(shell $(TOP_DIR)/scripts/avr/vusb_str USB_CFG_VENDOR_NAME $(CONFIG_USB_CFG_VENDOR_NAME) len ))

$(info ==> $(CFG_LEN))

commonflags-$(CONFIG_CONTRIB_VUSB)+=-DUSB_CFG_VENDOR_ID=$(U_VID) 
commonflags-$(CONFIG_CONTRIB_VUSB)+=-DUSB_CFG_DEVICE_ID=$(U_PID)
commonflags-$(CONFIG_CONTRIB_VUSB)+=-DUSB_CFG_DEVICE_VERSION=$(U_VER)

define vusb_def_string

$$(eval $$(shell $(TOP_DIR)scripts/avr/vusb_str $(1) $$(CONFIG_$(1)) str ))
$$(eval $$(shell $(TOP_DIR)scripts/avr/vusb_str $(1) $$(CONFIG_$(1)) len ))

commonflags-$(CONFIG_CONTRIB_VUSB)+="-D$(1)=$$($(1))"
commonflags-$(CONFIG_CONTRIB_VUSB)+=-D$(1)_LEN=$$($(1)_LEN)        
endef


$(eval $(call vusb_def_string,USB_CFG_VENDOR_NAME))
$(eval $(call vusb_def_string,USB_CFG_DEVICE_NAME))
$(eval $(call vusb_def_string,USB_CFG_SERIAL_NUMBER))