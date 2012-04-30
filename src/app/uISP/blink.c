#include <arch/antares.h>
#include <generated/usbconfig.h>
#include <arch/vusb/usbdrv.h>

uchar usbFunctionSetup(uchar data[8]) {

	
}

ANTARES_APP(blink_test)
{
	usbInit();
	while (1)
	{
	usbPoll();
	}
}