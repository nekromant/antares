#include <lib/earlycon.h>
#include <lib/printk.h>
#include <stdlib.h>

extern struct early_console g_early_console;
static char initialized = 0;


static inline void early_console_checkinit() {
	if (!initialized && g_early_console.init) { 
		initialized++;
		g_early_console.init();
#ifdef CONFIG_LIB_EARLYCON_ANNOUNCE
		printk("earlycon: early console enabled\n");
#endif
	}
}

void early_putc(char c) {
	if (c == '\n')
		early_putc('\r');
	early_console_checkinit();
	g_early_console.txchar(c);
}

int early_getc() {
	early_console_checkinit();
	if (NULL == g_early_console.rxchar)
		return -1;
	return g_early_console.rxchar();
}

