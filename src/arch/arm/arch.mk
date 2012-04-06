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
export CC CXX LD AR AS OBJCOPY DISAS OBJDUMP SIZE COMPILER_TOOLS

CFLAGS=$(call unquote,$(CONFIG_CFLAGS)) -include $(SRCDIR)/include/generated/autoconf.h
LDFLAGS=$(call unquote,$(CONFIG_LDFLAGS))
ASFLAGS=$(call unquote,$(CONFIG_ASFLAGS))
OBJCOPYFLAGS=-Obinary

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

ifeq ($(CONFIG_CC_LDFILE),y)
CFLAGS+=-Os
endif


CFLAGS+=-Wall
ifeq ($(CONFIG_GCC_PARANOID_WRN),y)
CFLAGS+=-Werror
endif

CFLAGS+=-I$(SRCDIR)/include

ASFLAGS+=$(COMMONFLAGS)
CFLAGS+=$(COMMONFLAGS)
LDFLAGS+=$(COMMONFLAGS)

#TODO: STM32 abstraction


ifeq ($(CONFIG_MCU_STM32),y)
include $(SRCDIR)/src/arch/arm/stm32/arch.mk
endif

BUILDGOALS=$(IMAGENAME).bin $(IMAGENAME).lss

builtin:
	$(Q)$(MAKE) OBJDIR=$(SRCDIR)/src SRCDIR=$(SRCDIR) TMPDIR=$(TMPDIR) -f make/Makefile.build -r build

$(IMAGENAME).elf: $(TMPDIR)/ldfile.lds builtin
	$(SILENT_LD) $(CC) -T $(TMPDIR)/ldfile.lds -o $(@) $(SRCDIR)/src/built-in.o $(LDFLAGS) 

$(IMAGENAME).bin: $(IMAGENAME).elf
	$(SILENT_OBJCOPY) $(OBJCOPY) $(OBJCOPYFLAGS) $< $(@) 

$(IMAGENAME).lss: $(IMAGENAME).elf
	$(SILENT_DISAS) $(OBJDUMP) -h -S $< > $@

PHONY+=builtin