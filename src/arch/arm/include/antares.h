//Top level header
//This sucks in the mcu-headers
#ifndef _ANTARES_H
#define _ANTARES_H


#define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))
#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))
#define __must_be_array(a) BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))

#ifdef CONFIG_ANTARES_STARTUP

#define ANTARES_INIT_LOW(fn) \
 __attribute__((naked))\
__attribute__((__section__(".text.antares_init_low"))) void fn(void)

#define ANTARES_INIT_HIGH(fn) \
  __attribute__((naked))\
__attribute__((__section__(".text.antares_init_high"))) void fn(void)

//TODO: Move this shit away once we have multitasking
#define ANTARES_APP(fn) \
  __attribute__((naked))\
__attribute__((__section__(".text.antares_app"))) void fn(void)

#define ANTARES_FINISH(fn) \
  __attribute__((naked))\
__attribute__((__section__(".text.antares_app_end"))) void fn(void)

#else

#define ANTARES_INIT_LOW(fn) void fn(void)
#define ANTARES_INIT_HIGH(fn) void fn(void)
#define ANTARES_APP(fn) void fn(void)


#endif

//This is a dummy define, ANTARES_LINK_RULE will be parsed 
//via scripts to determine required link order
#define ANTARES_DEPENDS(foo)  


#endif
