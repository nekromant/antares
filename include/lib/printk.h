#ifndef PRINTK_H
#define PRINTK_H

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 4
#endif
 
#ifndef COMPONENT
#define COMPONENT "???"
#endif

#define log(level, ...) __dbg_ # level (__VA_ARGS__)

#define err(...) printk(COMPONENT ": " __VA_ARGS__)
#define warn(...) __dbg_0(__VA_ARGS__)
#define info(...) __dbg_1(__VA_ARGS__)
#define dbg(...) __dbg_2(__VA_ARGS__)



#if DEBUG_LEVEL > 0 
#define __dbg_0(...) printk(COMPONENT ": " __VA_ARGS__)
#define D_WARN_ENABLED
#else
#define __dbg_0(...) { }
#endif

#if DEBUG_LEVEL > 1 
#define __dbg_1(...) printk(COMPONENT ": " __VA_ARGS__)
#define D_INFO_ENABLED
#else
#define __dbg_1(...) { }
#endif

#if DEBUG_LEVEL > 2 
#define __dbg_2(...) printk(COMPONENT ": " __VA_ARGS__)
#define D_DBG_ENABLED
#else
#define __dbg_2(...) { }
#endif

#if DEBUG_LEVEL > 3 
#define __dbg_3(...) printk(COMPONENT ": " __VA_ARGS__)
#else
#define __dbg_3(...) { }
#endif


void printk(const char *fmt, /*args*/ ...);

#endif
