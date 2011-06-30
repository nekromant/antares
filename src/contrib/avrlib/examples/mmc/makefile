# Makefile for mmctest
# Pascal Stang

########### change this lines according to your project ##################
#put the name of the target mcu here (at90s8515, at90s8535, attiny22, atmega603 etc.)
	MCU = atmega163
#	MCU = atmega323
#	MCU = atmega128

#put the name of the target file here (without extension)
	TRG	= mmctest

#put your C sourcefiles here 

	AVRLIB_SRC = buffer.c uart.c rprintf.c timer.c vt100.c spi.c mmc.c debug.c

	SRC = $(TRG).c

#put additional assembler source file here
	ASRC =

#additional libraries and object files to link
	LIB	=

#additional includes to compile
	INC	= 

#assembler flags
	ASFLAGS = -Wa, -gstabs

#compiler flags
	CPFLAGS	= -g  -Os -Wall -Wstrict-prototypes -I$(AVRLIB) -Wa,-ahlms=$(<:.c=.lst)

#linker flags
	LDFLAGS = -Wl,-Map=$(TRG).map,--cref

	
########### you should not need to change the following line #############
include $(AVRLIB)/make/avrproj_make
		  
###### dependecies, add any dependencies you need here ###################
buffer.o		: buffer.c		buffer.h
uart2.o		: uart2.c		uart2.h		global.h
rprintf.o	: rprintf.c		rprintf.h
a2d.o			: a2d.c			a2d.h
timer128.o	: timer128.c	timer128.h	global.h
lcd.o			: lcd.c			lcd.h			global.h
i2c.o			: i2c.c			i2c.h			global.h
spi.o			: spi.c			spi.h			global.h
rtc.o			: rtc.c			rtc.h			global.h
$(TRG).o		: $(TRG).c						global.h
