
###### Define directories to build ######

SUBDIRS =	a2d			\
			ads7828		\
			ads7870		\
			basic_io	\
			cmdline		\
			ds1631		\
			encoder		\
			extint		\
			glcd		\
			gps			\
			i2c			\
			lcd			\
			mmc			\
			netstack	\
			pulse		\
			rprintf		\
			servo		\
			spyglass	\
			stxetx		\
			timer		\

#SUBDIRS = $(dir ./*)

###### Define build rules ######

.PHONY: examples $(SUBDIRS)
examples: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY : all
all : examples

.PHONY : clean
clean : examples
	rm -vf $(addsuffix /AVRProject.ncb, $(SUBDIRS))
	rm -vf $(addsuffix /AVRProject.opt, $(SUBDIRS))
	rm -vf $(addsuffix /AVRProject.plg, $(SUBDIRS))
	rm -vf ../*.lst
	rm -vf ../ccrma/*.lst
	rm -vf ../megaio/*.lst
	rm -vf ../net/*.lst
	rm -vf ../rsl/*.lst

