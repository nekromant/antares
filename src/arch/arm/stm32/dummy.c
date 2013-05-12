#include <arch/antares.h>
#include "misc.h"

ANTARES_INIT_LOW(__disable_irqs__) 
{
	__disable_irq();
}

ANTARES_INIT_HIGH(__enable_irqs) 
{
	__enable_irq();
}

/* Dummy. 
 * Required to cause link errors if antares startup code 
 * is enabled and another main() is present. The rest of the magic 
 * is done in assembly 
 */

int main()
{
	return 0;
}

