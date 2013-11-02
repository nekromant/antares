/* This is the generic fallback, ANTARES startup
 * can't be supported via sdcc at the time of
 * writing without too many hacks
 */

#include <generic/antares.h>

#ifdef CONFIG_ARCH_8051_STC
#include <arch/stc.h>
#endif

#define ANTARES_DISABLE_IRQS()  EA=0
#define ANTARES_ENABLE_IRQS()   EA=1
