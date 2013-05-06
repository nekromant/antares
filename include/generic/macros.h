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


#define min_t(type, x, y) ({					\
			type __min1 = (x);			\
			type __min2 = (y);			\
			__min1 < __min2 ? __min1: __min2; })


#define max_t(type, x, y) ({					\
			type __max1 = (x);			\
			type __max2 = (y);			\
			__max1 > __max2 ? __max1: __max2; })

/* Just in case */
#ifndef NULL
#define NULL 0
#endif

#endif

