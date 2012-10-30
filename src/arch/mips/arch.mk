LD_NO_COMBINE=y

#Set our build goals
BUILDGOALS=$(IMAGENAME).bin $(IMAGENAME).lss

ifeq ($(CONFIG_MCU_1890),y)
include $(ANTARES_DIR)/src/arch/mips/1890/arch.mk
endif

$(TMPDIR)/ldfile.lds: $(GCC_LDFILE_IN)
	$(SILENT_GEN) cat "$^" | $(CC) -E -P -include $(TOPDIR)/include/generated/autoconf.h $(GFLAGS) -include $(ANTARES_DIR)/include/lib/sizes.h - > $(@)

