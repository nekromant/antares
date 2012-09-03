#ifndef _ANTARES_H
#define _ANTARES_H

/* We use .initX sections for low and high init functions
 * APPS are called one by one in the main()
 */

#define ANTARES_INIT_LOW(fn) \
 __attribute__((naked))\
__attribute__((__section__(".init5"))) void fn(void)

#define ANTARES_INIT_HIGH(fn) \
  __attribute__((naked))\
__attribute__((__section__(".init7"))) void fn(void)

#define ANTARES_APP(fn) \
  __attribute__((naked))\
__attribute__((__section__(".init8"))) void fn(void)

#define ANTARES_FINISH(fn) \
  __attribute__((naked))\
__attribute__((__section__(".fini0"))) void fn(void)


#endif
