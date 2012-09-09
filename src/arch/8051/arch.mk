BUILDGOALS=$(IMAGENAME).ihx $(IMAGENAME).bin

# Do not combine objects into one built-in.o
# This screws up things on avr and breaks ANTARES_* macros. Not supported by some compilers (sdcc)
# So it's 100% safe to set this to y

LD_NO_COMBINE=y

COMMONFLAGS+=-mmcs51
#TODO: menuconfigg'ed stack model and other shit
COMMONFLAGS+=--model-small



ifeq ($(CONFIG_ARCH_8051_STC),y)
include $(ANTARES_DIR)/src/arch/8051/stc/arch.mk
endif

$(IMAGENAME).bin: $(IMAGENAME).ihx
	$(SILENT_HEX2BIN)hex2bin $(<)
