SRCDIR=antares
GITURL=https://nekromant@github.com/nekromant/antares.git
OBJDIR=.
TMPDIR=tmp
TOPDIR=.

project_sources=src

$(info $(ANTARES_DIR))

-include antares/Makefile

ifeq ($(ANTARES_DIR),)
antares:
	git clone $(GITURL) $(SRCDIR)
else
antares:
	ln -sf $(ANTARES_DIR) $(SRCDIR)
endif


