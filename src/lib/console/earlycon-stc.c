#include <arch/stc.h>
#include <stdio.h>
#include <arch/lib/stc/iap.h>
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

static int serial_getchar()
{
	int v;
	while (!RI);;
	v = (int) SBUF;
	RI=0;
	return v;
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
#ifdef CONFIG_LIB_EARLYCON_STC_USE_P1
	AUXR|=0x80; /* Remap uart */
#endif
}


struct early_console g_early_console = {
	.rxchar = serial_getchar,
	.txchar = serial_putchar,
	.init = serial_init,		
	.havechar = serial_avail,
};
