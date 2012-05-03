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
%.tab.c: %.y
	bison -l -b $* -p $(notdir $*) $<
#	cp $@ $@_shipped

lex.%.c: %.l
	flex -L -P$(notdir $*) -o$@ $<
#	cp $@ $@_shipped

%.hash.c: %.gperf
	gperf < $< > $@
#	cp $@ $@_shipped


$(obj)/zconf.tab.o: $(src)/lex.zconf.c $(src)/zconf.hash.c

$(obj)/kconfig_load.o: $(src)/lkc_defs.h

#Guess we won't need this stuff
$(obj)/zconf.tab.c: $(src)/zconf.y

$(obj)/lex.zconf.c: $(src)/zconf.l

$(obj)/zconf.hash.c: $(src)/zconf.gperf

$(obj)/lkc_defs.h: $(src)/lkc_proto.h
	sed < $< > $@ 's/P(\([^,]*\),.*/#define \1 (\*\1_p)/'


$(obj)/%.o: $(src)/%.c
	 $(HOST_CC) -c $(HOST_EXTRACFLAGS) $< -o $@

$(obj)/conf: $(addprefix $(obj)/,$(conf-objs))
	 $(HOST_CC) $^ -o $@ $(HOST_LOADLIBES)

	
$(obj)/mconf: $(addprefix $(obj)/,$(mconf-objs))
	 $(HOST_CC) $^ -o $@ $(HOST_LOADLIBES)

kconfig-clean:
	-rm $(obj)/*.o
	-rm $(obj)/lxdialog/*.o
	-rm $(src)/zconf.tab.c* $(src)/lex.zconf.c* $(src)/zconf.hash.c*

versionupdate:
	$(Q)$(src)/config --set-str VERSION_MAJOR "$(VERSION_MAJOR)"
	$(Q)$(src)/config --set-str VERSION_MINOR "$(VERSION_MINOR)"
	$(Q)$(src)/config --set-str VERSION_CODENAME "$(VERSION_CODENAME)"
	$(Q)$(src)/config --set-str VERSION_GIT $(VERSION_GIT) 
	$(SILENT_VER) $(src)/config --set-str VERSION_STRING "$(VERSION_MAJOR).$(VERSION_MINOR), $(VERSION_CODENAME)"
	
  
menuconfig: $(obj)/mconf versionupdate collectinfo
	$< $(Kconfig)
	@echo "Version information updated, configuration is now complete"
	

config: $(obj)/conf
	$< --oldaskconfig $(Kconfig)

oldconfig: $(obj)/conf
	$< --$@ $(Kconfig)

silentoldconfig: $(obj)/conf
	$(Q)mkdir -p include/generated
	$(Q)mkdir -p include/config

	$< --$@ $(Kconfig)

switch_profile: $(obj)/mconf  
	-cp .config .config.switch_save
	$(src)/kcnf_list gen profiles/ > profiles.kcnf.inc
	$< profiles.kcnf
	$(src)/kcnf_list process_profile profiles
	
set_version: $(obj)/mconf  
	KCONFIG_CONFIG=.version $<  $(KVersion)

select_defconfig: $(obj)/mconf
	-cp .config .config.switch_save
	$(src)/kcnf_list fgen defconfigs/ > defconfig.kcnf.inc
	$< defconfig.kcnf 
	$(src)/kcnf_list process_defconfig defconfigs
