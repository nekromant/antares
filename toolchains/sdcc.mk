TOOL_PREFIX=sd

CC       := $(TOOL_PREFIX)cc
CXX      := $(TOOL_PREFIX)cpp
LD       := $(TOOL_PREFIX)ld
AR       := $(TOOL_PREFIX)ar
AS       := $(TOOL_PREFIX)cc

O=rel

COMPILER_TOOLS=CC="$(CC)" CXX="$(CXX)" LD="$(LD)" AR="$(AR)" AS="$(AS)" OBJCOPY="$(OBJCOPY)" OBJDUMP="$(OBJDUMP)" DISAS="$(DISAS)" SIZE="$(SIZE)"


#GENDEPFLAGS = -MD -MP -MF $$(@).d

#sdcc does NOT support -include directive. Bad

CFLAGS+=$(call unquote,$(CONFIG_SDCC_CFLAGS))
CFLAGS+=--model-small
CFLAGS+=-mmcs51
LDFLAGS+=$(call unquote,$(CONFIG_SDCC_LDFLAGS))

CFLAGS+=--model-small

CFLAGS+=--code-loc 0x2000 --data-loc 0x30 --xram-loc 0x6000


ASFLAGS+=$(call unquote,$(CONFIG_SDCC_ASFLAGS))


CFLAGS+=-I$(TOPDIR)/include -I$(ANTARES_DIR)/include
ASFLAGS+=$(COMMONFLAGS) $(GENDEPFLAGS)
CFLAGS+=$(COMMONFLAGS) $(GENDEPFLAGS)

export CC CXX LD AR AS OBJCOPY DISAS OBJDUMP SIZE COMPILER_TOOLS LD_NO_COMBINE
export ASFLAGS CFLAGS LDFLAGS ELFFLAGS 

builtin:
	$(SILENT_INFO) Building antares library code and startup
	$(Q) mkdir -p $(OBJDIR)/build/app
	$(Q)$(MAKE) OBJDIR=$(abspath $(OBJDIR)/build) SRCDIR=$(ANTARES_DIR)/src \
	-C $(ANTARES_DIR)/src \
	TMPDIR=$(TMPDIR) -f $(ANTARES_DIR)/make/Makefile.build -r build
	$(SILENT_INFO) Building application code
	$(Q)$(MAKE) OBJDIR=$(abspath $(OBJDIR)/build/app) SRCDIR=$(TOPDIR)/$(project_sources) \
	-C $(TOPDIR)/$(project_sources) \
	TMPDIR=$(TMPDIR) -f $(ANTARES_DIR)/make/Makefile.build -r build

ifneq ($(LD_NO_COMBINE),y)
$(IMAGENAME).ihx:  builtin
	$(SILENT_LD) $(CC) $(CFLAGS) $(OBJDIR)/build/built-in.o  
else
$(IMAGENAME).ihx: builtin
	$(SILENT_LD) $(CC) $(CFLAGS) $(LDFLAGS) -o $(@) \
	`$(ANTARES_DIR)/scripts/parseobjs $(TOPDIR)/build/built-in.rel` \
	`$(ANTARES_DIR)/scripts/parseobjs $(TOPDIR)/build/app/built-in.rel`
endif

$(IMAGENAME).bin: $(IMAGENAME).elf
	$(SILENT_OBJCOPY) $(OBJCOPY) $(OBJCOPYFLAGS) $< $(@) 

$(IMAGENAME).lss: $(IMAGENAME).elf
	$(SILENT_DISAS) $(OBJDUMP) -h -S $< > $@

PHONY+=builtin
