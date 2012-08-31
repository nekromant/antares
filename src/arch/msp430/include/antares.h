//Top level header
//This sucks in the mcu-headers
#ifndef _ANTARES_H
#define _ANTARES_H

#ifdef CONFIG_ARCH_MSP430

//We use .initX sections for low and high init functions
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
