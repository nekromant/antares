TOOL_PREFIX=$(call unquote,$(CONFIG_TOOLCHAIN_PREFIX))

CC       := $(TOOL_PREFIX)gcc
CPP	 := $(TOOL_PREFIX)cpp
CXX      := $(TOOL_PREFIX)g++
LD       := $(TOOL_PREFIX)ld
AR       := $(TOOL_PREFIX)ar
AS       := $(TOOL_PREFIX)gcc
OBJCOPY  := $(TOOL_PREFIX)objcopy
DISAS    := $(TOOL_PREFIX)objdump
OBJDUMP  := $(TOOL_PREFIX)objdump
SIZE     := $(TOOL_PREFIX)size
BIN2HEX  := $(TOOL_PREFIX)bin2hex


# This is for portability across sdcc and states the naming of intermediate files
# sdcc has this set to .rel (and that's quite retarded)
O=o


COMPILER_TOOLS:=\
		BIN2HEX="$(BIN2HEX)" \
		CC="$(CC)" \
		CXX="$(CXX)" \
		LD="$(LD)" \
		AR="$(AR)" \
		AS="$(AS)" \
		OBJCOPY="$(OBJCOPY)" \
		OBJDUMP="$(OBJDUMP)" \
		DISAS="$(DISAS)" \
		SIZE="$(SIZE)"

GENDEPFLAGS = -MD -MP -MF $$(@).d


CFLAGS+=$(call unquote,$(CONFIG_CFLAGS)) -include $(TOPDIR)/include/generated/autoconf.h
LDFLAGS+=$(call unquote,$(CONFIG_LDFLAGS))
ASFLAGS+=$(call unquote,$(CONFIG_ASFLAGS))
ELFFLAGS+=$(call unquote,$(CONFIG_ELFFLAGS))


#FixMe: Hack. Removing it breaks vusb (WTF?) Need --std=gnu99 globally
ASFLAGS+=$(CFLAGS)
CFLAGS+=--std=gnu99
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

ifeq ($(CONFIG_GCC_G),y)
COMMONFLAGS+=-g
endif


ifeq ($(CONFIG_GCC_LM),y)
ELFFLAGS+=-lm
endif

ifeq ($(CONFIG_GCC_FPIC),y)
 CFLAGS+=-fPIC
 LDFLAGS+=-fPIC
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


export CC CXX LD AR AS OBJCOPY DISAS OBJDUMP SIZE COMPILER_TOOLS 
export LD_NO_COMBINE BIN2HEX
export ASFLAGS CFLAGS LDFLAGS ELFFLAGS GENDEPFLAGS

builtin: $(TOPDIR)/include/generated/autoconf.h
	$(Q) mkdir -p $(OBJDIR)/build/app
ifeq ($(CONFIG_NEED_GENERATE),y)
	$(SILENT_INFO) Generating required headers
	$(Q)$(MAKE) OBJDIR=$(abspath $(OBJDIR)/build/app) SRCDIR=$(TOPDIR)/$(project_sources) \
	-C $(abspath $(OBJDIR)/build/app) \
	TMPDIR=$(TMPDIR) -f $(ANTARES_DIR)/make/Makefile.build -r generate
	$(Q)$(MAKE) OBJDIR=$(abspath $(OBJDIR)/build) SRCDIR=$(ANTARES_DIR)/src \
	-C $(abspath $(TOPDIR)/build) \
	TMPDIR=$(TMPDIR) -f $(ANTARES_DIR)/make/Makefile.build -r generate
endif
	$(SILENT_INFO) Building application code
	$(Q)$(MAKE) OBJDIR=$(abspath $(OBJDIR)/build/app) SRCDIR=$(TOPDIR)/$(project_sources) \
	-C $(abspath $(OBJDIR)/build/app) \
	TMPDIR=$(TMPDIR) -f $(ANTARES_DIR)/make/Makefile.build -r build
	$(SILENT_INFO) Building antares library code and startup
	$(Q)$(MAKE) OBJDIR=$(abspath $(OBJDIR)/build) SRCDIR=$(ANTARES_DIR)/src \
	-C $(abspath $(TOPDIR)/build) \
	TMPDIR=$(TMPDIR) -f $(ANTARES_DIR)/make/Makefile.build -r build


ifeq ($(CONFIG_BUILD_VERBOSE),y)
_debug_printout:
	@echo "======== Listing objects to be linked in ========="
	@echo "`$(ANTARES_DIR)/scripts/parseobjs $(TOPDIR)/build/built-in.o`"
	@echo "=================================================="
	@echo "`$(ANTARES_DIR)/scripts/parseobjs $(TOPDIR)/build/app/built-in.o`"
	@echo "=================================================="
else
_debug_printout:
	@echo > /dev/null
endif


ifneq ($(LD_NO_COMBINE),y)
%.elf %.so: $(GCC_LDFILE) builtin $(before-link)
	$(SILENT_LD) $(LD) -o $(@) $(OBJDIR)/build/built-in.o $(ELFFLAGS)

else
%.elf %.so: $(GCC_LDFILE) builtin _debug_printout $(before-link)
	echo $(CC) -o $(@) \
	`$(ANTARES_DIR)/scripts/parseobjs $(TOPDIR)/build/built-in.o` \
	`$(ANTARES_DIR)/scripts/parseobjs $(TOPDIR)/build/app/built-in.o` \
	$(ELFFLAGS)

	$(SILENT_LD) $(CC) -o $(@) \
	`$(ANTARES_DIR)/scripts/parseobjs $(TOPDIR)/build/built-in.o` \
	`$(ANTARES_DIR)/scripts/parseobjs $(TOPDIR)/build/app/built-in.o` \
	$(ELFFLAGS)
endif


$(IMAGENAME).bin: $(IMAGENAME).elf
	$(SILENT_OBJCOPY) $(OBJCOPY) $(OBJCOPYFLAGS) $< $(@) 

$(IMAGENAME).lss: $(IMAGENAME).elf
	$(SILENT_DISAS) $(OBJDUMP) -h -S $< > $@

PHONY+=builtin _debug_printout
