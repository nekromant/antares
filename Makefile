

#TODO: This really sucks
#I guess, I'll need to fix that later
Kbuild:=Kconfig
obj:=kconfig
src:=kconfig
Kconfig:=./kcnf
KVersion:=./version.kcnf

PHONY+=deftarget deploy build collectinfo clean

#overrride with actual version information
-include .config
-include .version
SRCDIR=.
OBJDIR=.
TOPDIR=.
TMPDIR=tmp
IMAGENAME=$(call unquote,$(CONFIG_IMAGE_DIR))/$(call unquote,$(CONFIG_IMAGE_FILENAME))


CONFIG_MAKE_DEFTARGET := $(subst ",, $(CONFIG_MAKE_DEFTARGET))

all: $(CONFIG_MAKE_DEFTARGET)
	@echo "Default target $(CONFIG_MAKE_DEFTARGET) remade"

include make/host.mk
include tmp/arch.mk
include make/Makefile.lib
include make/Makefile.collect

# -include src/arch/$(ARCH)/arch.mk
include kconfig/kconfig.mk

export SRCDIR TMPDIR IMAGENAME ARCH TOPDIR

clean: 
# 	$(MAKE) OBJDIR=$(SRCDIR)/src SRCDIR=$(SRCDIR) TMPDIR=$(TMPDIR) -f make/Makefile.build -r clean
	$(Q)find . -iname *.o| while read line; do rm "$$line"; done
	$(Q)rm tmp/*


mrproper: clean kconfig-clean 
	@echo "Мистер пропёр - веселей, в сырцах чисто в 3 раза быстрей!"

# build: collectinfo silentoldconfig collectinfo $(BUILD_PREREQS) 
# 	$(Q)$(MAKE) OBJDIR=$(SRCDIR)/src SRCDIR=$(SRCDIR) TMPDIR=$(TMPDIR) -f make/Makefile.build -r build

build: collectinfo silentoldconfig collectinfo $(BUILDGOALS)
	
	
deploy: build
	$(Q)$(MAKE) -f make/Makefile.deploy $(call unquote,$(CONFIG_DEPLOY_DEFTARGET))
	@echo "Your Antares firmware is now deployed"

deploy-%: build
	$(Q)$(MAKE) -f $(SRCDIR)/make/Makefile.deploy $*
	@echo "Your Antares firmware is now deployed"
	#run post-deployment
	$(Q)$(MAKE) -f $(SRCDIR)/make/Makefile.deploy post
	
deploy-help:
	make -f make/Makefile.deploy help

	
.PHONY: $(PHONY)