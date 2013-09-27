#include <msp430.h>
#include <stdio.h>
#include <stdlib.h>
#include <arch/lib/delaylib.h>

#define DELAY_FUNC(macro,measure,mul)			\
	void delay_ ## measure (unsigned long int m) {	\
		m = m * mul;				\
		while(m--)				\
			__delay_cycles(macro);		\
	}


#ifdef CONFIG_MSP430_DELAYLIB_DYNAMIC

#define CYCLES_PER_UNIT (unsigned long int) CONFIG_MSP430_DELAYLIB_UNITSIZE
static unsigned long int UNITS_PER_S;
static unsigned long int UNITS_PER_MS;
static unsigned long int UNITS_PER_US;

void delaylib_set_clock(unsigned long int f) 
{
	UNITS_PER_S  = f / CYCLES_PER_UNIT;
	UNITS_PER_MS = f / CYCLES_PER_UNIT / 1000UL;
	UNITS_PER_US = f / CYCLES_PER_UNIT / 1000000UL; 
}



DELAY_FUNC(CYCLES_PER_UNIT,ms, UNITS_PER_MS);
DELAY_FUNC(CYCLES_PER_UNIT,us, UNITS_PER_US);
DELAY_FUNC(CYCLES_PER_UNIT,s, UNITS_PER_S);

#else

#define CYCLES_PER_S  (CONFIG_MSP430_DELAYLIB_FRQ)
#define CYCLES_PER_MS (CONFIG_MSP430_DELAYLIB_FRQ / 1000UL)
#define CYCLES_PER_US (CONFIG_MSP430_DELAYLIB_FRQ / 1000000UL)

DELAY_FUNC(CYCLES_PER_MS, ms,1);
DELAY_FUNC(CYCLES_PER_S, s, 1);


#endif

#ifdef CONFIG_MSP430_DELAYLIB_SUMMARY
void delaylib_print_summary()
{
#ifdef CONFIG_MSP430_DELAYLIB_DYNAMIC
	printf("delaylib: running in dynamic mode\n");
	printf("delaylib: unit size %lu cycles\n", CYCLES_PER_UNIT);
	printf("delaylib: units_per_s %lu units_per_ms %lu units_per_us %lu\n",
	       UNITS_PER_S, UNITS_PER_MS, UNITS_PER_US);
#else
	printf("delaylib: running in static mode\n");
	printf("delaylib: cycles_per_s %lu cycles_per_ms %lu cycles_per_us %lu\n",
	       CYCLES_PER_S, CYCLES_PER_MS, CYCLES_PER_US);
#endif
}

#endif



