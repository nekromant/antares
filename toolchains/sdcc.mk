TOOL_PREFIX=sd

CC       := $(TOOL_PREFIX)cc
CXX      := $(TOOL_PREFIX)cpp
LD       := $(TOOL_PREFIX)ld
AR       := $(TOOL_PREFIX)ar
AS       := $(TOOL_PREFIX)cc

O=rel

COMPILER_TOOLS=CC="$(CC)" CXX="$(CXX)" LD="$(LD)" AR="$(AR)" AS="$(AS)" OBJCOPY="$(OBJCOPY)" OBJDUMP="$(OBJDUMP)" DISAS="$(DISAS)" SIZE="$(SIZE)"

GENDEPFLAGS=-Wp "-MD -MP -MF $$(@).d"

CFLAGS+=-Wp "-include $(TOPDIR)/include/generated/autoconf.h"
CFLAGS+=$(call unquote,$(CONFIG_SDCC_CFLAGS))
LDFLAGS+=$(call unquote,$(CONFIG_SDCC_LDFLAGS))

#TODO: Move this to subarch makefiles, and hook to kconfig
#COMMONFLAGS+=--code-loc 0x2000 --data-loc 0x30 --xram-loc 0x6000


ASFLAGS+=$(call unquote,$(CONFIG_SDCC_ASFLAGS))


CFLAGS+=-I$(TOPDIR)/include -I$(ANTARES_DIR)/include
CFLAGS+=$(COMMONFLAGS) $(GENDEPFLAGS)
ASFLAGS+=$(COMMONFLAGS) $(GENDEPFLAGS)
LDFLAGS+=$(COMMONFLAGS)

export CC CXX LD AR AS OBJCOPY DISAS OBJDUMP SIZE COMPILER_TOOLS LD_NO_COMBINE
export ASFLAGS CFLAGS LDFLAGS ELFFLAGS 


#We now need to parse opts from kconfig

builtin: $(TOPDIR)/include/generated/autoconf.h
	$(SILENT_INFO) Building antares library code and startup
	$(Q) mkdir -p $(OBJDIR)/build/app
	$(Q) $(MAKE) OBJDIR=$(abspath $(OBJDIR)/build) SRCDIR=$(ANTARES_DIR)/src \
	-C $(OBJDIR)/build \
	TMPDIR=$(TMPDIR) -f $(ANTARES_DIR)/make/Makefile.build -r build
	$(SILENT_INFO) Building application code
	$(Q) $(MAKE) OBJDIR=$(abspath $(OBJDIR)/build/app) SRCDIR=$(TOPDIR)/$(project_sources) \
	-C $(abspath $(OBJDIR)/build/app) \
	TMPDIR=$(TMPDIR) -f $(ANTARES_DIR)/make/Makefile.build -r build


$(IMAGENAME).ihx: builtin
	$(SILENT_LD) $(CC) $(LDFLAGS) -o $(@) \
	`$(ANTARES_DIR)/scripts/parseobjs $(TOPDIR)/build/built-in.rel` \
	`$(ANTARES_DIR)/scripts/parseobjs $(TOPDIR)/build/app/built-in.rel`

PHONY+=builtin
