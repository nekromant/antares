#ifdef CONFIG_TOOLCHAIN_SDCC
#define __attribute_packed__

#endif

#ifndef CONFIG_TOOLCHAIN_GCC
#define __attribute_packed__  __attribute__ ((packed))

#endif
