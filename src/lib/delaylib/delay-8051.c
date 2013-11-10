#include <arch/delay.h>
#include <stdint.h>

/*  
 * This is a very lousy delay assuming a 1T core. 
 * Do not rely on it for ANY precision 
 */

/*
 * cycle count == 2 + 4*c. Max 1026;
 */
static void delay_cycles_small(unsigned char c)
{
	c;
        __asm
	mov     r0, dpl /* 2 cycles */
	00001$: 
	nop /* 1 cycle */
        nop /* 1 cycle */
        djnz    r0, 00001$ /* 2 cycles */
        __endasm;
}

/*  
 * TODO: Rewrite this in asm, to add precision
 * The 32bit var handling introduces extra delay
 * that is REALLY visible
 * 
 */
void delay_4cycles(uint32_t c)
{
	if (c<0xff)
		delay_cycles_small((unsigned char) c);
	else {
		c = c >> 8;
		while(c--) 
			delay_cycles_small(0xff);
	}
}
