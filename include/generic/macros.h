#ifndef __MACRO_H
#define __MACRO_H

/* Borrowed from the linux kernel. Don't try to understand it. */

#ifdef CONFIG_TOOLCHAIN_GCC

#define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))
#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))
#define __must_be_array(a) BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))

#else

/* SDCC will never support original, gcc-like voodoo */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#endif


#endif

