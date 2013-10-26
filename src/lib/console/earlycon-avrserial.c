#include <lib/earlycon.h>
#include <lib/printk.h>
#include <stdio.h>
#include <avr/io.h>
#include <generic/macros.h>

#define SERIAL_NUM  CONFIG_LIB_EARLYCON_AVRSERIAL_PORT
#define BAUD CONFIG_LIB_EARLYCON_AVRSERIAL_BAUD

#if defined(UDR)
/* We have only one serial port */
#define REG_UDR UDR
#define REG_UCSRA UCSRA
#define REG_UBRRH UBRRH
#define REG_UBRRL UBRRL
#define BIT_TXC TXC
#define BIT_U2X UX2
#define BIT_RXC RXC
#define BIT_UDRE UDRE
#else
/* We use Nth port */
#define REG_UDR PPCAT(UDR, SERIAL_NUM)
#define REG_UCSRA PPCAT3(UCSR, SERIAL_NUM, A)
#define REG_UBRRH PPCAT3(UBRR, SERIAL_NUM, H)
#define REG_UBRRL PPCAT3(UBRR, SERIAL_NUM, L)
#define BIT_TXC PPCAT(TXC, SERIAL_NUM)
#define BIT_U2X PPCAT(U2X, SERIAL_NUM)
#define BIT_RXC PPCAT(RXC, SERIAL_NUM)
#define BIT_UDRE PPCAT(UDRE, SERIAL_NUM)
#endif


static void serial_putchar(char c)
{
	loop_until_bit_is_set(REG_UCSRA, BIT_UDRE);
	REG_UDR = c;
}

static int serial_getchar( void )
{
	loop_until_bit_is_set(REG_UCSRA, BIT_RXC);
	return (int) REG_UDR;	
}

static int serial_avail( void )
{
	return bit_is_set(REG_UCSRA, BIT_RXC);
}

static void serial_init(void)
{
#include <util/setbaud.h>
	REG_UBRRH = UBRRH_VALUE;
	REG_UBRRL = UBRRL_VALUE;
#if USE_2X
	REG_UCSRA |= (1 << BIT_U2X);
#else
	REG_UCSRA &= ~(1 << BIT_U2X);
#endif

#ifdef CONFIG_LIB_EARLYCON_ANNOUNCE
	printk("earlycon: using avr serial port %u at %u bps\n", 
	       (unsigned int) SERIAL_NUM, 
	       (unsigned int) BAUD
		);
#endif
}


struct early_console g_early_console = {
	.havechar = serial_avail,
	.rxchar = serial_getchar,
	.txchar = serial_putchar,
	.init = serial_init		
};
