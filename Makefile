include host.mk
#TODO: This really sucks
#I guess, I'll need to fix that later
Kbuild:=Kconfig
obj:=kconfig
src:=kconfig
Kconfig:=./kcnf

obj-y:=t

.PHONY: deftarget

subdirs-y:=arch/avr
	

#Suck in the current .config file
-include .config

CONFIG_MAKE_DEFTARGET := $(subst ",, $(CONFIG_MAKE_DEFTARGET))

all: $(CONFIG_MAKE_DEFTARGET)
	@echo "Default target $(CONFIG_MAKE_DEFTARGET) remade"

#This parses the CONFIG_ARCH into something readable and 
#includes arch-specific files
include arch/arch.mk

include kconfig/kconfig.mk


#Now the fun stuff
#To make this thing work like the real thing

setupsymlinks:
	ln -sf ../arch/$(ARCH)/include include/arch

build: versionupdate setupsymlinks
	$(MAKE) -f Makefile.build -r build
	
clean: kconfig-clean 
	$(MAKE) -f Makefile.build -r clean

deploy: build .deployed
	@echo "Your Antares firmware is now deployed"
	