#include <arch/delay.h>

void delay_4cycles(uint32_t cycles)
{
	uint32_t nloop;
	nloop = cycles + 1; /* Each loop takes about 4 cycles including nop +1 is here to avoid delay of 0 */
	asm volatile(
		"1: " "\n\t"
		" nop " "\n\t"
		" subs.w %0, %0, #1 " "\n\t"
		" bne 1b " "\n\t"
		: "=r" (nloop)
		: "0"(nloop)
		: "r3"
		);
}
