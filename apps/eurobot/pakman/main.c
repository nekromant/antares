#include <arch/antares.h>
ANTARES_INIT_LOW(pakman_init)
 {
  asm("nop;");
// //   //code here
 }

ANTARES_APP(pakman_init2)
 {
  asm("nop;");
  DDRB=0xfa;
// //   //code here
 }


 ANTARES_APP(pakman_main)
 {
   PORTB=0xff;
 }
