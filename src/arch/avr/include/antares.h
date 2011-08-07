//Top level header
//This sucks in the mcu-headers
#ifndef _ANTARES_H
#define _ANTARES_H
#include <generated/autoconf.h>

#ifdef CONFIG_ARCH_AVR
//Suck in all the default headers
#ifndef __ASSEMBLER__
#include <string.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#endif

//#include <string.h>

//In case of avr we use .initX sections for low and high init functions

//Suck in all the default headers

//In case of avr we use .initX sections for low and high init functions
//APPS are called one by one in the main()
#define ANTARES_INIT_LOW(fn) \
 __attribute__((naked))\
__attribute__((__section__(".init5"))) void fn(void)

#define ANTARES_INIT_HIGH(fn) \
  __attribute__((naked))\
__attribute__((__section__(".init7"))) void fn(void)

//TODO: Move this shit away once we have multitasking
#define ANTARES_APP(fn) \
  __attribute__((naked))\
__attribute__((__section__(".init8"))) void fn(void)

//This is a dummy define, ANTARES_LINK_RULE will be parsed 
//via bash to determine link order
#define ANTARES_LINK_RULE(foo)  

#define ANTARES_FINISH(fn) \
  __attribute__((naked))\
__attribute__((__section__(".fini0"))) void fn(void)



#endif


#endif
