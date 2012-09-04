BUILDGOALS=$(IMAGENAME).ihx

# Do not combine objects into one built-in.o
# This screws up things on avr and breaks ANTARES_* macros. Not supported by some compilers (sdcc)
# So it's 100% safe to set this to y

LD_NO_COMBINE=y

COMMONFLAGS+=-mgbz80


