__Stack_Size = 1024 ;

PROVIDE ( _Stack_Size = __Stack_Size ) ;

__Stack_Init = _estack  - __Stack_Size ;

/*"PROVIDE" allows to easily override these values from an object file or the commmand line.*/
PROVIDE ( _Stack_Init = __Stack_Init ) ;

/*
There will be a link error if there is not this amount of RAM free at the end.
*/
_Minimum_Stack_Size = 0x100 ;


/* Memory Spaces Definitions */

MEMORY
{
  RAM (xrw) : ORIGIN = 0x80000000 , LENGTH = 0x1000000
}

/* higher address of the user mode stack */
_estack = 0x81000000;

/* include the sections management sub-script for FLASH mode */

/* Sections Definitions */

SECTIONS
{   
    /* the program code is stored in the .text section, which goes to Flash */
    .text :
    {
            . = ALIGN(4);
            
        *(.text)                   /* remaining code */
        *(.text.*)                   /* remaining code */
        *(.rodata)                 /* read-only data (constants) */
        *(.rodata*)
        *(.glue_7)
        *(.glue_7t)

            . = ALIGN(4);
         _etext = .;
            /* This is used by the startup in order to initialize the .data secion */
         
    } >RAM

 /* used by the startup to initialize data */
  _sidata = .;

    /* This is the initialized data section
    The program executes knowing that the data is in the RAM
    but the loader puts the initial values in the FLASH (inidata).
    It is one task of the startup to copy the initial values from FLASH to RAM. */
    .data  : AT ( _sidata )
    {
            . = ALIGN(4);
        /* This is used by the startup in order to initialize the .data secion */
        _sdata = . ;
        
        *(.data)
        *(.data.*)

            . = ALIGN(4);
            /* This is used by the startup in order to initialize the .data secion */
         _edata = . ;
    } >RAM
    
    /* This is the uninitialized data section */
    .bss :
    {
            . = ALIGN(4);
        /* This is used by the startup in order to initialize the .bss secion */
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(COMMON)
        
            . = ALIGN(4);
            /* This is used by the startup in order to initialize the .bss secion */
         _ebss = . ;
 __bss_end__ = _ebss;
    } >RAM
    
    PROVIDE ( end = _ebss );
    PROVIDE ( _end = _ebss );
    
    /* This is the user stack section 
    This is just to check that there is enough RAM left for the User mode stack
    It should generate an error if it's full.
     */
    ._usrstack :
    {
            . = ALIGN(4);
        _susrstack = . ;
        
        . = . + _Minimum_Stack_Size ;
        
            . = ALIGN(4);
        _eusrstack = . ;
    } >RAM    
}
