#include <arch/lib/delaylib.h>

void delay_us(unsigned int t)
{
	unsigned int i;
	while(t--)
		for(i = 0;i < CONFIG_8051_DELAYMHZ;i++)
		{
			__asm;
	                nop;
	                __endasm;
		}
}

void delay_ms(unsigned int t)
{
	while(t --)
		delay_us(1000);
}
