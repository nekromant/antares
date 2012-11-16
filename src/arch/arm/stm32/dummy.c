#include <arch/antares.h>


/* Dummy. 
 * Required to cause link errors if antares startup code 
 * is enabled and another main() is present. The rest of the magic 
 * is done in assembly 
 */

int main()
{
	return 0;
}

