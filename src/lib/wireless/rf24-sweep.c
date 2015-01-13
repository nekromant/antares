#include <lib/RF24.h>
#include <lib/nRF24L01.h>

#include <string.h>
#include <arch/delay.h>
#include <arch/antares.h>

#include <lib/printk.h>


/** 
 * Initialize the sweeper struct with zero values and set up
 * slave rf24 instance to work with 
 *
 * @param s sweeper struct to initialize
 * @param r rf24 instance to act upon
 */
void rf24_sweeper_init(struct rf24_sweeper *s, struct rf24 *r) 
{
	s->r = r;
	memset(s->values, 0, sizeof(s->values));
	rf24_write_register(r, EN_AA, 0x0); /* disable auto-ack */
}

/** 
 * Scan all the available channels for carrier/rpd
 * Results are stored in the sweeper struct 
 * 
 * @param s sweeper instance to work with
 * @param loops number of loops to perform the sweep
 */
void rf24_sweep(struct rf24_sweeper *s, int loops)
{
	while (loops--)
	{
		int i = RF24_NUM_CHANNELS;
		while (i--)
		{
			/* Select this channel */
			rf24_write_register(s->r, RF_CH, i);
			
			// Listen for a little
			rf24_start_listening(s->r);
			delay_us(128);
			rf24_stop_listening(s->r);
			
			/* Did we get a carrier? */
#ifdef CONFIG_LIB_RF24_SWEEP_RPD
			if ( rf24_test_rpd(s->r) )
#else
			if ( rf24_test_carrier(s->r) )
#endif
				s->values[i]++;
			
		}
	}
	
}

/** 
 * Dump sweep results to debug console followed by \r
 * 
 * @param s sweeper instance
 */
void rf24_sweep_dump_results(struct rf24_sweeper *s)
{
	int i = 0;
	while ( i < RF24_NUM_CHANNELS )
	{
		printk("%x", min_t(uint8_t, 0xf, s->values[i] & 0xf));
		++i;
	}
	printk("\r");
}

/** 
 * Dump convenience header with channel numbers to console
 * 
 */
void rf24_sweep_dump_header()
{
	int i = 0;
	printk("\n\n\n");
	while ( i < RF24_NUM_CHANNELS )
	{
		printk("%x",i>>4);
		++i;
	}
	printk("\n");
	i = 0;
	while ( i < RF24_NUM_CHANNELS )
	{
		printk("%x",i&0xf);
		++i;
	}
	i=0;
	printk("\n");
	while ( i < RF24_NUM_CHANNELS )
	{
		printk("-");
		++i;
	}
	printk("\n");
}
