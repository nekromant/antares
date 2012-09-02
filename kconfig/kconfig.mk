.PHONY: conf mconf

HOST_CC:=gcc

.SECONDEXPANSION: 
check-lxdialog:= $(src)/lxdialog/check-lxdialog.sh

HOST_EXTRACFLAGS = $(shell $(check-lxdialog) -ccflags)
HOST_LOADLIBES   = $(shell $(check-lxdialog) -ldflags $(HOST_CC))
HOST_EXTRACFLAGS += -DLOCALE

lxdialog := lxdialog/checklist.o lxdialog/util.o lxdialog/inputbox.o
lxdialog += lxdialog/textbox.o lxdialog/yesno.o lxdialog/menubox.o
conf-objs	:= conf.o  zconf.tab.o
mconf-objs     := mconf.o zconf.tab.o $(lxdialog)

VERSION_MAJOR := $(shell $(src)/config --file "$(SRCDIR)/.version" --state VERSION_MAJOR )
VERSION_MINOR := $(shell $(src)/config --file "$(SRCDIR)/.version" --state VERSION_MINOR )
VERSION_CODENAME := $(shell $(src)/config --file "$(SRCDIR)/.version" --state VERSION_CODENAME )
VERSION_GIT = $(shell GIT_DIR=$(SRCDIR)/.git git rev-parse --verify HEAD --exec-path=$(src))


#	cp $@ $@_shipped
# TODO: use shipped versions here

%.tab.c: %.y
	$(SILENT_BISON)bison -l -b $* -p $(notdir $*) $<

lex.%.c: %.l
	$(SILENT_FLEX)flex -L -P$(notdir $*) -o$@ $<

%.hash.c: %.gperf
	$(SILENT_GPERF)gperf < $< > $@

$(obj)/zconf.tab.o: $(src)/lex.zconf.c $(src)/zconf.hash.c

$(obj)/kconfig_load.o: $(src)/lkc_defs.h

#Guess we won't need this stuff
$(obj)/zconf.tab.c: $(src)/zconf.y

$(obj)/lex.zconf.c: $(src)/zconf.l

$(obj)/zconf.hash.c: $(src)/zconf.gperf

$(obj)/lkc_defs.h: $(src)/lkc_proto.h
	$(Q)sed < $< > $@ 's/P(\([^,]*\),.*/#define \1 (\*\1_p)/'


$(obj)/%.o: $(src)/%.c
	 $(SILENT_HOSTCC)$(HOST_CC) -c $(HOST_EXTRACFLAGS) $< -o $@

$(obj)/conf: $(addprefix $(obj)/,$(conf-objs))
	 $(SILENT_HOSTCC)$(HOST_CC) $^ -o $@ $(HOST_LOADLIBES)


$(obj)/mconf: $(addprefix $(obj)/,$(mconf-objs))
	 $(SILENT_HOSTCC)$(HOST_CC) $^ -o $@ $(HOST_LOADLIBES)

kconfig-clean:
	$(Q)-rm $(obj)/*.o
	$(Q)-rm $(obj)/lxdialog/*.o
	$(Q)-rm $(src)/zconf.tab.c* $(src)/lex.zconf.c* $(src)/zconf.hash.c*

versionupdate:
	$(Q)$(src)/config --set-str VERSION_MAJOR "$(VERSION_MAJOR)"
	$(Q)$(src)/config --set-str VERSION_MINOR "$(VERSION_MINOR)"
	$(Q)$(src)/config --set-str VERSION_CODENAME "$(VERSION_CODENAME)"
	$(Q)$(src)/config --set-str VERSION_GIT $(VERSION_GIT) 
	$(SILENT_VER) $(src)/config --set-str VERSION_STRING "$(VERSION_MAJOR).$(VERSION_MINOR), $(VERSION_CODENAME)"

#FixThis: Properly learn the kconfig deps dance 
menuconfig: collectinfo $(obj)/mconf $(obj)/conf versionupdate
	$(Q) $(obj)/mconf $(Kconfig)
	$(Q)mkdir -p include/generated
	$(Q)mkdir -p include/config
	$(Q)$< --$@ $(Kconfig)
	@echo "Version information updated, configuration is now complete"


$(TOPDIR)/include/config/auto.conf: $(deps_config) .config
	$(SILENT_INFO) "Config changed, running silentoldconfig"
	$(Q)$(MAKE) -f $(ANTARES_DIR)/Makefile silentoldconfig

config: $(obj)/conf
	$(Q)$< --oldaskconfig $(Kconfig)

oldconfig: $(obj)/conf
	$(Q)$< --$@ $(Kconfig)

silentoldconfig: $(obj)/conf
	$(Q)mkdir -p include/generated
	$(Q)mkdir -p include/config
	$(Q)$< --$@ $(Kconfig)

set_version: $(obj)/mconf  
	$(Q)KCONFIG_CONFIG=$(ANTARES_DIR)/.version $<  $(KVersion)
