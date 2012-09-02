#include <msp430.h>
#include <arch/lib/delaylib.h>


#ifdef CONFIG_MSP430_DELAYLIB_SCALING
static int mul=1;
static int div=1;
void delay_scale(int mmul, int mdiv)
{
	mul=mmul;
	div=mdiv;
}

#define __SCALEM(m) m*=mul; m/=div;
#else
#define __SCALEM(m) ;
#endif


#define DELAY_FUNC(macro,measure) \
void delay_ ## measure (int m) {\
	__SCALEM(m);			\
	while(m--)			\
		__delay_cycles(macro);	\
}

DELAY_FUNC(CYCLES_PER_MS,ms);
DELAY_FUNC(CYCLES_PER_MS,us);
DELAY_FUNC(CYCLES_PER_MS,ns);
DELAY_FUNC(CYCLES_PER_MS,s);









