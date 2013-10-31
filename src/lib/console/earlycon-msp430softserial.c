#include <arch/antares.h>
#include <msp430.h>
#include <lib/earlycon.h>
#include <generic/macros.h>


/* These defines will screw us up, so ditch 'em */
#undef DIR
#undef OUT

#define PDIR             PPCAT3(P,CONFIG_LIB_EARLYCON_MSP430_SOFTSERIAL_PORT,DIR)
#define POUT             PPCAT3(P,CONFIG_LIB_EARLYCON_MSP430_SOFTSERIAL_PORT,OUT)
#define BAUDRATE         CONFIG_LIB_EARLYCON_MSP430_SOFTSERIAL_BAUD
#define TXD_PIN          (1<<CONFIG_LIB_EARLYCON_MSP430_SOFTSERIAL_PIN)
#define CYCLES_PER_BIT   (F_CPU / BAUDRATE)


#define ZERO POUT  &=~TXD_PIN; __delay_cycles(CYCLES_PER_BIT);
#define ONE  POUT  |=TXD_PIN; __delay_cycles(CYCLES_PER_BIT);
#define PUTFUNC launchpad_putchar

#include "softserial_helper.c"

static void launchpad_init()
{
	PDIR |= TXD_PIN;
	POUT |= TXD_PIN;
}

struct early_console g_early_console = {
	.txchar = launchpad_putchar,
	.init = launchpad_init,		
};
