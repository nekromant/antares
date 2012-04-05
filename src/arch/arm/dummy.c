#include <arch/antares.h>



ANTARES_APP(antares_first_app)
{
	//Dummy
}

ANTARES_FINISH(antares_exit)
{
	asm("b antares_first_app"); 
}

int main_()
{
	int a=5;
	a+=5;
	return 0; //dummy
}
