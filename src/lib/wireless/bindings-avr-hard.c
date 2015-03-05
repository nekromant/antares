#include <arch/antares.h> 
#include <avr/io.h> 
#include <arch/delay.h>
#include <lib/RF24.h>
#include <string.h>
#include <avr/boot.h>
#include <avr/io.h>
#include <avr/wdt.h>

#define CSN_PIN   (1<<CONFIG_LIB_RF24_CSN_PIN)
#define  CE_PIN   (1<<CONFIG_LIB_RF24_CE_PIN)
#define SPI_PORTX PORTB
#define SPI_DDRX  DDRB
#define SPI_MOSI  CONFIG_LIB_RF24_SPI_MOSI_PIN
#define SPI_MISO  CONFIG_LIB_RF24_SPI_MISO_PIN
#define SPI_SCK   CONFIG_LIB_RF24_SPI_SCK_PIN
#define SPI_SS    CONFIG_LIB_RF24_SPI_SS_PIN

static void set_csn(int level) 
{
	if (level) 
		CSN_PORT |= CSN_PIN;
	else
		CSN_PORT &= ~CSN_PIN;
}

static void set_ce(int level) 
{
	if (level) 
		CE_PORT |= CE_PIN;
	else
		CE_PORT &= ~CE_PIN;
}


static void spi_set_speed(int speed)
{

}

static void spi_write(const uint8_t *data, uint8_t len)
{
	while(len--) {
		SPDR = *data++;
		while(!(SPSR & (1<<SPIF)));
	}
}

static void spi_read(uint8_t *data, uint8_t len)
{
	while(len--) {
		SPDR = 0xff;
		while(!(SPSR & (1<<SPIF)));
		*data++ = SPDR;
	}
}

static struct rf24 r = {
	.csn = set_csn,
	.ce  = set_ce, 
	.spi_set_speed = spi_set_speed, 
	.spi_write = spi_write,
	.spi_read  = spi_read
};

struct rf24 *g_radio = &r;

ANTARES_INIT_LOW(avr_hardspi_init)
{
	CSN_DDR |= CSN_PIN;
	CE_DDR  |=  CE_PIN;

	SPI_DDRX &= ~(1<<SPI_MISO);
	SPI_DDRX |= (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS);
	SPI_PORTX |= (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_MISO)|(1<<SPI_SS);
	

	SPCR = ((1<<SPE)|               /* SPI Enable */
		(0<<SPIE)|              /* SPI Interrupt Enable */
		(0<<DORD)|              /* Data Order (0:MSB first / 1:LSB first) */
		(1<<MSTR)|              /* Master/Slave select */   
		(0<<SPR1)|(0<<SPR0)|    /* SPI Clock Rate */
		(0<<CPOL)|              /* Clock Polarity (0:SCK low / 1:SCK hi when idle) */
		(0<<CPHA));             /* Clock Phase (0:leading / 1:trailing edge sampling) */

	SPSR = (1<<SPI2X);              /* Double Clock Rate */
}
