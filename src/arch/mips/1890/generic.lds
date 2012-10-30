OUTPUT_FORMAT("elf32-bigmips", "elf32-bigmips",
              "elf32-littlemips")
OUTPUT_ARCH(mips)

SEARCH_DIR(/CKCPB/local/H-sun/bt23b/lib);

SECTIONS
{
  /* Read-only sections, merged into text segment: */
  .text :
    {
    _ftext = . ;
    *(.text)
    } =0

  .rodata :
    {
    *(.rodata)
  _etext = .;
    }


  .data :
    {
    _fdata = . ;
    *(.data)
    }

/*  _gp = ALIGN(16) + 0x7ff0; */

  /* We want the small data sections together, so single-instruction offsets
     can access them all, and initialized data all before uninitialized, so
     we can shorten the on-disk segment size.  */

  .sdata :
    {
    _gp = . ;
    *(.sdata)
    }
  _edata  =  .;


  __bss_start = .;
  _fbss = .;

  .sbss :
    {
    *(.sbss)
    *(.scommon)
    }

  .bss :
  {
   *(.bss)
   *(COMMON)
  }
  _end = . ;

  .reginfo :
    {
    *(.reginfo)
    }

}
