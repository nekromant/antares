/* 
 * Antares generic stuff and prototypes
 */

#ifndef ANTARES_GENERIC_H
#define ANTARES_GENERIC_H

#ifndef __ASSEMBLER__
   #include <stdint.h>
   #include <generic/initcall.h>
#endif 

#include <generic/macros.h>
#include <generic/error.h>
#include <generic/compiler.h>


/* If arch didn't have any of these defined - take care */
#if !defined(ANTARES_APP)
 #define ANTARES_APP(fn) void antares_app_ ## fn()
#endif

#if !defined(ANTARES_INIT_LOW)
#define ANTARES_INIT_LOW(fn) void antares_init_low_ ## fn()
#endif

#if !defined(ANTARES_INIT_HIGH)
#define ANTARES_INIT_HIGH(fn) void antares_init_high_ ## fn()	      
#endif

#if !defined(ANTARES_FINISH)
#define ANTARES_FINISH(fn) void antares_finish_ ## fn()
#endif

#endif
