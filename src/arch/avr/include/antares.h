#ifndef _ANTARES_H
#define _ANTARES_H

#define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))
#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))
#define __must_be_array(a) BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))


//In case of avr we use .initX sections for low and high init functions
//APPS are called one by one in the main()
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
