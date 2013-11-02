/* 
 * Antares generic stuff and prototypes
 */

#ifndef ANTARES_GENERIC_H
#define ANTARES_GENERIC_H

#include <generic/macros.h>

#if !defined(CONFIG_ANTARES_STARTUP) && defined(CONFIG_ARCH_HAS_ANTARES_STARTUP)
#define ANTARES_APP(fn) void fn()
#define ANTARES_INIT_LOW(fn) void fn()
#define ANTARES_INIT_HIGH(fn) void fn()	      
#define ANTARES_FINISH(fn) void fn()
#endif

#endif
