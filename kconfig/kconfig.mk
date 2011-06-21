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

VERSION_MAJOR := $(shell kconfig/config --state VERSION_MAJOR)
VERSION_MINOR := $(shell kconfig/config --state VERSION_MINOR)
VERSION_CODENAME := $(shell kconfig/config --state VERSION_CODENAME)

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


%.o: %.c
	 $(HOST_CC) -c $(HOST_EXTRACFLAGS) $< -o $@

$(obj)/conf: $(addprefix $(obj)/,$(conf-objs))
	 $(HOST_CC) $^ -o $@ $(HOST_LOADLIBES)


$(obj)/mconf: $(addprefix $(obj)/,$(mconf-objs))
	 $(HOST_CC) $^ -o $@ $(HOST_LOADLIBES)

kconfig-clean:
	-rm $(obj)/*.o
	-rm $(src)/zconf.tab.c* $(src)/lex.zconf.c* $(src)/zconf.hash.c*

versionupdate:
	$(obj)/config --set-str VERSION_STRING "$(VERSION_MAJOR).$(VERSION_MINOR), $(VERSION_CODENAME) "
	$(obj)/config --set-str VERSION_GIT $(shell git rev-parse --verify HEAD)
  
menuconfig: $(obj)/mconf versionupdate
	$< $(Kconfig)
	@echo "Version information updated, configuration is now complete"
	

config: $(obj)/conf
	$< --oldaskconfig $(Kconfig)

oldconfig: $(obj)/conf
	$< --$@ $(Kconfig)

silentoldconfig: $(obj)/conf
	$(Q)mkdir -p src/include/generated
	$< --$@ $(Kconfig)

switch_profile: $(obj)/mconf  
	-cp .config .config.switch_save
	$(src)/kcnf_list gen profiles/ > profiles.kcnf.inc
	$< profiles.kcnf
	$(src)/kcnf_list process_profile profiles
	
select_defconfig: $(obj)/mconf
	-cp .config .config.switch_save
	$(src)/kcnf_list fgen defconfigs/ > defconfig.kcnf.inc
	$< defconfig.kcnf 
	$(src)/kcnf_list process_defconfig defconfigs
