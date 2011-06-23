include make/host.mk
include make/Makefile.lib
#TODO: This really sucks
#I guess, I'll need to fix that later
Kbuild:=Kconfig
obj:=kconfig
src:=kconfig
Kconfig:=./kcnf

obj-y:=t

PHONY+=deftarget deploy build

subdirs-y:=arch apps
	

#Suck in the current .config file
-include .config

CONFIG_MAKE_DEFTARGET := $(subst ",, $(CONFIG_MAKE_DEFTARGET))

all: $(CONFIG_MAKE_DEFTARGET)
	@echo "Default target $(CONFIG_MAKE_DEFTARGET) remade"

#This parses the CONFIG_ARCH into something readable and 
#includes arch-specific files
include src/arch/arch.mk
include kconfig/kconfig.mk


#Now the fun stuff
#To make this thing work like the real thing

setupsymlinks:
	#ln -sf ../arch/$(ARCH)/include src/include/arch

build-%:
	$(MAKE) -f make/Makefile.build $*

build: versionupdate setupsymlinks silentoldconfig
	$(MAKE) -f make/Makefile.build -r build
	$(call colorize,$(t_grn))
	@echo "Antares build is now complete"
	@echo "Inspect the files under $(IMAGES_DIR) and have fun"
	$(call colorize,$(col_rst))
	
clean: 
	$(MAKE) -f make/Makefile.build -r clean

mrproper: clean kconfig-clean 
	@echo "Мистер пропёр - веселей, в сырцах чисто в 3 раза быстрей!"

deploy: build
	make -f make/Makefile.deploy $(call unquote,$(CONFIG_DEPLOY_DEFTARGET))
	@echo "Your Antares firmware is now deployed"

deploy-%: build
	make -f make/Makefile.deploy $*
	@echo "Your Antares firmware is now deployed"

.PHONY: $(PHONY)
