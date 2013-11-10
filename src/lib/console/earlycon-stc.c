#include <arch/stc.h>
#include <stdio.h>
#include <arch/stc/iap.h>
#include <lib/earlycon.h>
#include <lib/printk.h>

#define br CONFIG_LIB_EARLYCON_STCUART_BAUD
#define MAKEBRT(baud) (256 - F_CPU/32/baud) 


static void serial_putchar(char c)
{
	TI=0;
	SBUF=c;
	while (!TI);;
	TI=0;
}

#ifdef CONFIG_LIB_EARLYCON_STCUART_IAPCHECK
static unsigned char icount=0;
static void iap_check(char c) {
	if (c == 0x7f) { 
		icount++;
		if (icount == CONFIG_LIB_EARLYCON_STCUART_IAPCNT)
			iap_reboot2isp();
	}
	else
		icount=0;
}  

#else
static void iap_check(char c) {
	c;
}  
#endif
static int serial_getchar()
{
	char c;
	while (!RI);;
	c = SBUF;
	RI=0;
	iap_check(c);
	return c;
}

static int serial_avail() 
{
	return RI;
}

static void serial_init()
{
	SCON = 0x50;
	BRT = MAKEBRT(br); 
	AUXR|=0x15;
#ifdef CONFIG_LIB_EARLYCON_STCUART_USE_P1
	AUXR|=0x80; /* Remap uart */
#endif
}


struct early_console g_early_console = {
	.rxchar = serial_getchar,
	.txchar = serial_putchar,
	.init = serial_init,		
	.havechar = serial_avail,
};
