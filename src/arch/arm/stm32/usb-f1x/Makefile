
CFLAGS+=-I$(TOPDIR)/include/generated

objects-$(CONFIG_HAS_OTG)+=\
otgd_fs_cal.o \
otgd_fs_dev.o \
otgd_fs_int.o \
otgd_fs_pcd.o 

objects-$(CONFIG_HAS_USB)+=\
usb_core.o \
usb_init.o \
usb_int.o \
usb_mem.o \
usb_regs.o \
usb_sil.o 


conf=$(TOPDIR)/include/generated/usb_conf.h
gen-y+=$(conf)

subdirs-$(CONFIG_STM32_USB_HI)+=highlevel

#The following voodoo takes care to generate a proper usb_conf.h
bits=CNTR_CTRM CNTR_DOVRM CNTR_WKUPM CNTR_ERRM CNTR_SUSPM CNTR_RESETM CNTR_SOFM
$(foreach bit,$(bits),\
	$(eval $(call check_and_append,imr,IMR_$(bit),$(bit)))\
)

endpoints=1 2 3 4 5 6 7

$(foreach ep,$(endpoints),\
$(eval $(call check_and_define,epcallbacks,USB_EP$(ep)_IN_E,EP$(ep)_IN_Callback,\
$(call unquote,$(CONFIG_USB_EP$(ep)_IN_HNDL)),NOP_Process))\
$(eval $(call check_and_define,epcallbacks,USB_OUT$(ep)_IN_E,EP$(ep)_OUT_Callback,\
$(call unquote,$(CONFIG_USB_EP$(ep)_OUT_HNDL)),NOP_Process))\
)

$(conf):
	$(SILENT_GEN)echo -e '$(call generate,c_header_start,USB_CONF)' > $(@)
	$(Q)echo -e '$(call generate,c_define_or,IMR_MSK, $(imr))' >> $(@)
	$(Q)echo -e '$(call generate,epcallbacks)' >> $(@)
	$(Q)echo -e '$(call generate,c_header_end)' >> $(@)

