#include <lib/earlycon.h>
#include <lib/printk.h>
#include <stdlib.h>

extern struct early_console g_early_console;
static char initialized = 0;


/* Initialise glue infrastructure */
extern void early_console_glue_init();

void early_console_checkinit() {
	if (!initialized && g_early_console.init) { 
		initialized++;
#ifndef CONFIG_LIB_EARLYCON_GLUE_NONE
		early_console_glue_init();
#endif
		g_early_console.init();
#ifdef CONFIG_LIB_EARLYCON_ANNOUNCE
		printk("earlycon: early console enabled\n");
#endif
	}
}

void early_putc(char c) {
#ifdef CONFIG_LIB_EARLYCON_ADDCR
	if (c == '\n')
		early_putc('\r');
#endif
	early_console_checkinit();
	g_early_console.txchar(c);
}

int early_getc() {
	early_console_checkinit();
	if (NULL == g_early_console.rxchar)
		return -1;
	return g_early_console.rxchar();
}


int early_avail() {
	early_console_checkinit();
	if (NULL == g_early_console.havechar)
		return -1;
	return g_early_console.havechar();
}


