include host.mk
#TODO: This really sucks
#I guess, I'll need to fix that later
Kbuild:=Kconfig
obj:=kconfig
src:=kconfig
Kconfig:=./kcnf


.PHONY: deftarget


	

#Suck in the current config file
-include .config

CONFIG_MAKE_DEFTARGET := $(subst ",, $(CONFIG_MAKE_DEFTARGET))

deftarget: $(CONFIG_MAKE_DEFTARGET)
	@echo "Default target $(CONFIG_MAKE_DEFTARGET) remade"

#This parses the CONFIG_ARCH into something readable and 
#includes arch-specific files
include arch/arch.mk




include kconfig/kconfig.mk