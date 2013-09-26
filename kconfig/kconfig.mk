#Test if antares dir is a git repo
VERSION_MAJOR := $(shell $(ANTARES_DIR)/kconfig/config --file "$(SRCDIR)/.version" --state VERSION_MAJOR )
VERSION_MINOR := $(shell $(ANTARES_DIR)/kconfig/config --file "$(SRCDIR)/.version" --state VERSION_MINOR )
VERSION_CODENAME := $(shell $(ANTARES_DIR)/kconfig/config --file "$(SRCDIR)/.version" --state VERSION_CODENAME )

# We can be a git repo or a tarball, here goes the tricky part
# In case of tarball use the git revision info from .version
VERSION_GIT=$(shell test -d "$(ANTARES_DIR)/.git" && \
	GIT_DIR="$(ANTARES_DIR)/.git" git rev-parse --verify HEAD --exec-path=$(src) ||\
	$(ANTARES_DIR)/kconfig/config --file "$(SRCDIR)/.version" --state VERSION_GIT)

VERSION_GIT_DEP=$(shell test -d "$(ANTARES_DIR)/.git" && echo "$(ANTARES_DIR)/.git" )


define KCONF_WARNING

			$(t_red)WARNING!$(col_rst)

	Antares now uses kconfig-frontends instead of bundled hacky kconfig
		(Bundled version sucked anyway) 
	You do not seem to have these installed.
	You can download kconfig-frontends from their homepage here
	http://ymorin.is-a-geek.org/projects/kconfig-frontends

endef


HAVE_KCONFIG=$(shell which kconfig-conf 2>/dev/null || echo no)

ifeq ($(HAVE_KCONFIG),no)
$(info $(KCONF_WARNING))
$(error Sorry)
endif

$(TMPDIR)/.version: $(ANTARES_GIT_DEP) $(ANTARES_DIR)/.version
	$(Q)$(ANTARES_DIR)/kconfig/config --set-str VERSION_MAJOR "$(VERSION_MAJOR)"
	$(Q)$(ANTARES_DIR)/kconfig/config --set-str VERSION_MINOR "$(VERSION_MINOR)"
	$(Q)$(ANTARES_DIR)/kconfig/config --set-str VERSION_CODENAME "$(VERSION_CODENAME)"
	$(Q)$(ANTARES_DIR)/kconfig/config --set-str VERSION_GIT "$(VERSION_GIT)" 
	$(SILENT_VER) $(ANTARES_DIR)/kconfig/config --set-str VERSION_STRING "$(VERSION_MAJOR).$(VERSION_MINOR), $(VERSION_CODENAME)"
	$(Q)touch $(@)

define frontend_template
$(1): collectinfo
	$$(Q) kconfig-$(2) $(Kconfig)
	$$(Q)echo "Antares configuration is now complete."
	$$(Q)echo "Run 'make build' to build everything now"
endef

$(eval $(call frontend_template,menuconfig,mconf))
$(eval $(call frontend_template,gconfig,gconf))
$(eval $(call frontend_template,qconfig,qconf))
$(eval $(call frontend_template,nconfig,nconf))


$(TOPDIR)/include/config/auto.conf: $(TMPDIR)/.configured
	$(Q)echo > /dev/null

$(TOPDIR)/include/generated/autoconf.h: $(TMPDIR)/.configured
	$(Q)echo > /dev/null


config: collectinfo
	$(Q)kconfig-conf --oldaskconfig $(Kconfig)

oldconfig: collectinfo
	$(Q)kconfig-conf --$@ $(Kconfig)

$(TMPDIR)/.configured: $(deps_config) .config
	$(SILENT_INFO) "Config changed, running silentoldconfig"
	$(Q)$(MAKE) -f $(ANTARES_DIR)/Makefile silentoldconfig
	$(Q)touch $(@)

silentoldconfig: collectinfo
	$(Q)mkdir -p include/generated
	$(Q)mkdir -p include/config
	$(Q)kconfig-conf --$@ $(Kconfig)

set_version:
	$(Q)$(ANTARES_DIR)/kconfig/config --file $(ANTARES_DIR)/.version --set-str VERSION_GIT "$(VERSION_GIT)"
	$(Q)KCONFIG_CONFIG=$(ANTARES_DIR)/.version kconfig-mconf $(KVersion)
	$(Q)echo "N.B. Remember to amend the version changes to the release commit"
