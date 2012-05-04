TOOL_PREFIX=$(call unquote,$(CONFIG_TOOLCHAIN_PREFIX))

CC       := $(TOOL_PREFIX)gcc
CXX      := $(TOOL_PREFIX)g++
LD       := $(TOOL_PREFIX)ld
AR       := $(TOOL_PREFIX)ar
AS       := $(TOOL_PREFIX)gcc
OBJCOPY  := $(TOOL_PREFIX)objcopy
DISAS    := $(TOOL_PREFIX)objdump
OBJDUMP  := $(TOOL_PREFIX)objdump
SIZE     := $(TOOL_PREFIX)size

COMPILER_TOOLS=CC="$(CC)" CXX="$(CXX)" LD="$(LD)" AR="$(AR)" AS="$(AS)" OBJCOPY="$(OBJCOPY)" OBJDUMP="$(OBJDUMP)" DISAS="$(DISAS)" SIZE="$(SIZE)"

CFLAGS+=$(call unquote,$(CONFIG_CFLAGS)) -include $(TOPDIR)/include/generated/autoconf.h
LDFLAGS+=$(call unquote,$(CONFIG_LDFLAGS))
ASFLAGS+=$(call unquote,$(CONFIG_ASFLAGS))

#FixMe: Hack
ASFLAGS+=$(CFLAGS)
OBJCOPYFLAGS+=-Obinary

#Let's parse optimisations from .config
#TODO: Move this to a separate file
ifeq ($(CONFIG_CC_OPT1),y)
CFLAGS+=-O1
endif

ifeq ($(CONFIG_CC_OPT0),y)
CFLAGS+=-O0
endif

ifeq ($(CONFIG_CC_OPT2),y)
CFLAGS+=-O2
endif

ifeq ($(CONFIG_CC_OPT3),y)
CFLAGS+=-O3
endif

ifeq ($(CONFIG_CC_OPTSZ),y)
CFLAGS+=-Os
endif

ifeq ($(CONFIG_GCC_NOSTDLIBS),y)
ELFFLAGS+=-nostdlib
endif

ifeq ($(CONFIG_GCC_LC),y)
ELFFLAGS+=-lc
endif

ifeq ($(CONFIG_GCC_LM),y)
ELFFLAGS+=-lm
endif

CFLAGS+=-Wall
ifeq ($(CONFIG_GCC_PARANOID_WRN),y)
CFLAGS+=-Werror
endif

#Voodoo to remove dead code from image
ifeq ($(CONFIG_GCC_STRIP),y)
#DEADCODE+=-Wl,-static
CFLAGS+=-fdata-sections -ffunction-sections
ELFFLAGS+=-Wl,--gc-sections -Wl,-s 
endif

CFLAGS+=-I$(TOPDIR)/include -I$(ANTARES_DIR)/include

ASFLAGS+=$(COMMONFLAGS)
CFLAGS+=$(COMMONFLAGS) 
LDFLAGS+=$(COMMONFLAGS)


ifneq ($(GCC_LDFILE),)
ELFFLAGS+=-T $(GCC_LDFILE)
endif

export CC CXX LD AR AS OBJCOPY DISAS OBJDUMP SIZE COMPILER_TOOLS LD_NO_COMBINE
export ASFLAGS CFLAGS LDFLAGS ELFFLAGS

builtin:
	mkdir -p $(OBJDIR)/build
	$(Q)$(MAKE) OBJDIR=$(abspath $(OBJDIR)/build) SRCDIR=$(ANTARES_DIR)/src \
	-C $(ANTARES_DIR)/src \
	TMPDIR=$(TMPDIR) -f $(ANTARES_DIR)/make/Makefile.build -r build


ifneq ($(LD_NO_COMBINE),y)
$(IMAGENAME).elf: $(GCC_LDFILE) builtin
	$(SILENT_LD) $(CC) $(ELFFLAGS) -o $(@) $(OBJDIR)/build/built-in.o 
else
$(IMAGENAME).elf: $(GCC_LDFILE) builtin
	$(SILENT_LD) $(CC) $(ELFFLAGS) -o $(@) `$(ANTARES_DIR)/scripts/parseobjs $(TOPDIR)/build/built-in.o`
endif

$(IMAGENAME).bin: $(IMAGENAME).elf
	$(SILENT_OBJCOPY) $(OBJCOPY) $(OBJCOPYFLAGS) $< $(@) 

$(IMAGENAME).lss: $(IMAGENAME).elf
	$(SILENT_DISAS) $(OBJDUMP) -h -S $< > $@

PHONY+=builtin