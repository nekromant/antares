# Makefile for AVR function library development and examples
# Author: Pascal Stang
#
# For those who have never heard of makefiles: a makefile is essentially a
# script for compiling your code.  Most C/C++ compilers in the world are
# command line programs and this is even true of programming environments
# which appear to be windows-based (like Microsoft Visual C++).  Although
# you could use AVR-GCC directly from the command line and try to remember
# the compiler options each time, using a makefile keeps you free of this
# tedious task and automates the process.
#
# For those just starting with AVR-GCC and not used to using makefiles,
# I've added some extra comments above several of the makefile fields which
# you will have to deal with.

########### change this lines according to your project ##################
#put the name of the target mcu here (at90s8515, at90s8535, attiny22, atmega603 etc.)
	MCU = atmega163
#	MCU = atmega323
#	MCU = atmega161
#	MCU = atmega128

#put the name of the target file here (without extension)
#  Your "target" file is your C source file that is at the top level of your code.
#  In other words, this is the file which contains your main() function.

	TRG = a2dtest

#put your C sourcefiles here 
#  Here you must list any C source files which are used by your target file.
#  They will be compiled in the order you list them, so it's probably best
#  to list $(TRG).c, your top-level target file, last.

	SRC = $(AVRLIB)/buffer.c $(AVRLIB)/uart.c $(AVRLIB)/rprintf.c $(AVRLIB)/timer.c $(AVRLIB)/a2d.c $(AVRLIB)/vt100.c $(TRG).c

#put additional assembler source file here
#  The ASRC line allows you to list files which contain assembly code/routines that
#  you would like to use from within your C programs.  The assembly code must be
#  written in a special way to be usable as a function from your C code.

	ASRC =

#additional libraries and object files to link
#  Libraries and object files are collections of functions which have already been
#  compiled.  If you have such files, list them here, and you will be able to use
#  use the functions they contain in your target program.

	LIB	=

#additional includes to compile
	INC	= 

#assembler flags
	ASFLAGS = -Wa, -gstabs

#compiler flags
	CPFLAGS	= -g -Os -Wall -Wstrict-prototypes -I$(AVRLIB) -Wa,-ahlms=$(<:.c=.lst)

#linker flags
	LDFLAGS = -Wl,-Map=$(TRG).map,--cref
#	LDFLAGS = -Wl,-Map=$(TRG).map,--cref -lm

	
########### you should not need to change the following line #############
include $(AVRLIB)/make/avrproj_make
		  
###### dependecies, add any dependencies you need here ###################
#  Dependencies tell the compiler which files in your code depend on which
#  other files.  When you change a piece of code, the dependencies allow
#  the compiler to intelligently figure out which files are affected and
#  need to be recompiled.  You should only list the dependencies of *.o 
#  files.  For example: uart.o is the compiled output of uart.c and uart.h
#  and therefore, uart.o "depends" on uart.c and uart.h.  But the code in
#  uart.c also uses information from global.h, so that file should be listed
#  in the dependecies too.  That way, if you alter global.h, uart.o will be
#  recompiled to take into account the changes.

buffer.o		: buffer.c		buffer.h
uart.o		: uart.c			uart.h		global.h
uart2.o		: uart2.c		uart2.h		global.h
rprintf.o	: rprintf.c		rprintf.h
a2d.o			: a2d.c			a2d.h
timer.o		: timer.c		timer.h		global.h
pulse.o		: pulse.c		pulse.h		timer.h	global.h
lcd.o			: lcd.c			lcd.h			global.h
i2c.o			: i2c.c			i2c.h			global.h
spi.o			: spi.c			spi.h			global.h
swpwm.o		: swpwm.c		swpwm.h		global.h
servo.o		: servo.c		servo.h		global.h
swuart.o		: swuart.c		swuart.h		global.h
tsip.o		: tsip.c			tsip.h		global.h
nmea.o		: nmea.c			nmea.h		global.h
vt100.o		: vt100.c		vt100.h		global.h
gps.o			: gps.c			gps.h			global.h
$(TRG).o		: $(TRG).c						global.h
