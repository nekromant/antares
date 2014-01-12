#ifndef _ANTARES_LIB_INTTYPES_H
#define _ANTARES_LIB_INTTYPES_H

/**
 * Platform-dependent inttypes.h
 * This file collects data about integer sizes
 * of current platform by including platform-dependent files
 *
 * TODO: fill this file with extra includes
 */

#if defined(CONFIG_ARCH_AVR)
    #include <stdint.h>
#elif defined(CONFIG_ARCH_ARM)
    #if defined(CONFIG_STM32F1X)
        #include <stm32f10x.h>
    #endif
#elif defined(CONFIG_ARCH_NATIVE)
    #include <inttypes.h>
#endif

#endif
