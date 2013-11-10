#include <msp430.h>
#include <stdio.h>
#include <stdlib.h>
#include <arch/delay.h>

#ifdef CONFIG_F_DYNAMIC

void delay_multi (uint32_t m, uint32_t mul) 
{
	m = m * mul;
	while(m--)
		__delay_cycles(CONFIG_MSP430_DELAYLIB_UNITSIZE);
}

#endif



