#include <arch/antares.h>
#include <intrinsics.h>

#ifdef CONFIG_F_DYNAMIC
/* Default to F_CPU from config */
uint32_t msp430_core_clock = CONFIG_F_CPU;

#endif

