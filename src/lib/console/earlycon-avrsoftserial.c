#include <arch/antares.h>
#include <avr/io.h>
#include <util/delay.h>
#include <lib/earlycon.h>
#include <generic/macros.h>


/* These defines will screw us up, so ditch 'em */

#define PORT              PPCAT(PORT,LIB_EARLYCON_AVR_SOFTSERIAL_PORT)
#define DDR               PPCAT(DDR,LIB_EARLYCON_AVR_SOFTSERIAL_PORT)
#define PIN               (1<<CONFIG_LIB_EARLYCON_AVR_SOFTSERIAL_PIN)
#define BAUDRATE          CONFIG_LIB_EARLYCON_AVR_SOFTSERIAL_BAUD

#define CYCLES_PER_BIT   (F_CPU / BAUDRATE)

#define ZERO  PORT&=~PIN; __builtin_avr_delay_cycles(CYCLES_PER_BIT);
#define ONE   PORT|=PIN; __builtin_avr_delay_cycles(CYCLES_PER_BIT);
#define PUTFUNC avrsoftserial_putchar

#include "softserial_helper.c"

static void avrsoftserial_init()
{
	DDR|=PIN;
	PORT|=PIN;
}

struct early_console g_early_console = {
	.txchar = avrsoftserial_putchar,
	.init = avrsoftserial_init,		
};
