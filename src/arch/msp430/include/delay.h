#ifndef ARCH_DELAY_H
#define ARCH_DELAY_H

#include <arch/antares.h>

#ifndef CONFIG_F_DYNAMIC

#define delay_s(m) \
	__delay_cycles(m * get_system_clock() );

#define delay_ms(m) \
	__delay_cycles(m * (get_system_clock() / 1000UL) );

#define delay_us(m) \
	__delay_cycles(m * (get_system_clock() / 1000000UL) );

#else /* CONFIG_F_DYNAMIC */

void delay_multi (uint32_t m, uint32_t mul);

#define delay_s(m) \
	delay_multi(m, (get_system_clock() / CONFIG_MSP430_DELAYLIB_UNITSIZE) );

#define delay_ms(m) \
	delay_multi(m, (get_system_clock() / CONFIG_MSP430_DELAYLIB_UNITSIZE / 1000UL) );

#define delay_us(m) \
	delay_multi(m, (get_system_clock() / CONFIG_MSP430_DELAYLIB_UNITSIZE / 1000000UL) );

#endif


#endif
