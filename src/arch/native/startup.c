#include <stdio.h>
#include <stdlib.h>
#include <arch/antares.h>


static struct antares_app *apps;

void antares_app_register (struct antares_app *app)
{
	if (!apps) {
		apps=app;
		return;
	}
	struct antares_app *itr = apps;
	while (itr->next) 
		itr = itr->next;
	itr->next = app;
}


int main() {
	if (!apps)
	{
		printf("OOPS: No antares apps found, exiting\n");
		exit(1);
	}
	while (1)
	{
		struct antares_app *itr = apps;
		while (itr) {
			itr->func();
			itr = itr->next;
		}
	}
}
