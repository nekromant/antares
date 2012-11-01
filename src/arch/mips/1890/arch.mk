ifeq ($(CONFIG_1890_LD_C),y)
GCC_LDFILE=$(ANTARES_DIR)/src/arch/mips/1890/cram.lds
else
GCC_LDFILE=$(ANTARES_DIR)/src/arch/mips/1890/generic.lds
endif

ASFLAGS=        -D_ASMLANGUAGE -pedantic-errors \
                -W -Wall -x assembler-with-cpp -fno-pic 

DATA_ADDR=      0x80110000
PROGRAM_ADR=    0x80100000
PROGRAM_STACK=  0x80100000

#Toolchain must be r3000-ready
CFLAGS+=        -msoft-float
ASFLAGS+=       -DPROGRAM_ADR=$(PROGRAM_ADR) -DPROGRAM_STACK=$(PROGRAM_STACK) 
ASFLAGS+=       -DDATA_ADDR=$(DATA_ADDR)

OBJCOPYFLAGS=--verbose -R .reginfo -R .sbss -R .bss
