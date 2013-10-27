#include <arch/antares.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <lib/stlinky.h>
#include <lib/earlycon.h>
#include <lib/printk.h>

static void stl_putchar(char c)
{
	stlinky_tx(&g_stlinky_term, &c, 1);
}

/* NOTE: 
 * We loose data here, if hosts sends us 
 * more than 1 byte at a time. Use this with care
 */

static int stl_getchar()
{
	char buf;
	stlinky_rx(&g_stlinky_term, &buf, 1);
	return (int) buf;
}

static int stl_avail()
{
	return stlinky_avail(&g_stlinky_term);
}

static void stl_init()
{
#ifdef CONFIG_LIB_EARLYCON_ANNOUNCE
	printk("earlycon: stlinky early console\n");
#endif
}

struct early_console g_early_console = {
	.havechar = stl_avail,
	.rxchar = stl_getchar,
	.txchar = stl_putchar,
	.init = stl_init		
};
