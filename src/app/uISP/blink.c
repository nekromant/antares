#include <arch/antares.h>


ANTARES_APP(blink_test)
{
	DDRC|=1<<2;
	while (1)
	{
	PORTC|=(1<<2);
	_delay_ms(1000);
	PORTC&=~(1<<2);
	_delay_ms(1000);
	
	}
}