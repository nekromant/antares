#include <arch/antares.h>
#define _DBG_ENABLED 1
#include <contrib/crumbo/debug.h>

ANTARES_INIT_LOW(pakman_init)
 {
  asm("nop;");
 //code here
 }


ANTARES_APP(pakman_mainapp)
 {
  
  //comm_send_pgms(PSTR("Main app"));
//   comm_sync();
//   _delay_ms(1000);
   DBG("Hello, world. Place your code in here!");
   DBG("Use #include <contrib/crumbo/file.h> for headers")
//   comm_sync();
//   _delay_ms(1000);
//   
// //   //code here
 }

