#include <arch/antares.h>
#include <lib/RF24.h>
#include <lib/nRF24L01.h>
#include <arch/delay.h>
#include <lib/panic.h>
#include <string.h>

#define DEBUG_LEVEL CONFIG_LIB_RF24_DEBUG
#define COMPONENT "rf24"
#include <lib/printk.h>
#include <stdint.h>


/* Agressive and hacky size optimisation, 
   mostly for avr
*/

#ifdef CONFIG_LIB_RF24_SIZEOPT
/* S
 *  r->csn(0) costs us 24 bytes 
 *  rf24_csn() costs us 4 bytes 
 *  ...
 *  PROFIT!
 */

extern struct rf24 *g_radio;

static void rf24_csn(uint8_t state) 
{
	g_radio->csn(state);
}
static void rf24_ce(uint8_t state) 
{
	g_radio->ce(state);
}

#if 0
static uint8_t rf24_spi_xfer(uint8_t d) 
{
	return g_radio->spi_xfer(d);
}
#endif

static  void rf24_spi_write(const uint8_t *data, uint8_t len)
{
	g_radio->spi_write(data, len);
}

static  void rf24_spi_read(uint8_t *data, uint8_t len)
{
	g_radio->spi_read(data, len);
}


#else
/* In case we're not that space-hungry */
#define rf24_csn(v)       r->csn(v)
#define rf24_ce(v)        r->ce(v)
#define rf24_spi_write(v,n) r->spi_write(v,n)
#define rf24_spi_read(v,n)  r->spi_read(v,n)
#endif


static void rf24_readout(struct rf24 *r, uint8_t cmd, uint8_t *data, uint8_t len)
{
	rf24_csn(0);
	rf24_spi_write(&cmd, 1);
	if (len)
		rf24_spi_read(data, len);
	rf24_csn(1);	
}

static void rf24_writeout(struct rf24 *r, uint8_t cmd, const uint8_t *data, uint8_t len)
{
	rf24_csn(0);
	rf24_spi_write(&cmd, 1);
	if (len)
		rf24_spi_write(data, len);
	rf24_csn(1);		
}


/**
 * Read a chunk of data in from a register
 *
 * @param r rf24 instance to act upon
 * @param reg Which register. Use constants from nRF24L01.h
 * @param buf Where to put the data
 * @param len How many bytes of data to transfer
 */
void rf24_readout_register(struct rf24 *r,
			      uint8_t reg, uint8_t* buf, uint8_t len)
{
	rf24_readout(r, (R_REGISTER | ( REGISTER_MASK & reg )), buf, len);
}

/**
 * Write a chunk of data to a register
 *
 * @param r rf24 instance to act upon
 * @param reg Which register. Use constants from nRF24L01.h
 * @param buf Where to get the data
 * @param len How many bytes of data to transfer
 */
void rf24_writeout_register(struct rf24 *r, uint8_t reg, const uint8_t* buf, uint8_t len)
{
	rf24_writeout(r, (W_REGISTER | ( REGISTER_MASK & reg )), buf, len);
}

/**
 * Write a single byte to a register
 *
 * @param r rf24 instance to act upon
 * @param reg Which register. Use constants from nRF24L01.h
 * @param value The new value to write
 */
void rf24_write_register(struct rf24 *r, uint8_t reg, uint8_t value)
{
	rf24_writeout(r, (W_REGISTER | ( REGISTER_MASK & reg )), &value, 1);
	trace("write_register(%02x,%02x)\n", reg, value);
}


/**
* Read single byte from a register
*
* @param r rf24 instance to act upon
* @param reg Which register. Use constants from nRF24L01.h
* @return Current value of register @p reg
*/
uint8_t rf24_read_register(struct rf24 *r, uint8_t reg)
{
	uint8_t result;
	rf24_readout(r, (R_REGISTER | ( REGISTER_MASK & reg )), &result, 1);
	trace("read_register(%02x,%02x)\n", reg, result);
	return result;
}


/**
 * Write the transmit payload
 *
 * The size of data written is the fixed payload size, see rf24_get_payload_size()
 *
 * @param r rf24 instance to act upon
 * @param buf Where to get the data
 * @param len Number of bytes to be sent
 */
void rf24_write_payload(struct rf24 *r, const void* buf, uint8_t len)
{
	uint8_t blanks[RF24_MAX_PAYLOAD];

	uint8_t cmd = W_TX_PAYLOAD;
	uint8_t data_len = min_t(uint8_t, len, r->payload_size);
	uint8_t blank_len = rf24_has_dynamic_payload(r) ? 0 : r->payload_size - data_len;
	dbg("Writing %u bytes %u blanks (len %d)\n", data_len, blank_len, len);
	memset(blanks, 0x0, blank_len);

	rf24_csn(0);
	rf24_spi_write( &cmd, 1 );
	rf24_spi_write(buf, data_len);
	rf24_spi_write(blanks, blank_len);  
	rf24_csn(1);

}

/**
 * Read the receive payload
 *
 * The size of data read is the fixed payload size, see rf24_get_payload_size()
 *
 * @param r rf24 instance to act upon
 * @param buf Where to put the data
 * @param len Maximum number of bytes to read
 */
void rf24_read_payload(struct rf24 *r, void* buf, uint8_t len)
{
	uint8_t cmd = R_RX_PAYLOAD;
	uint8_t blanks[RF24_MAX_PAYLOAD];
	uint8_t data_len = min_t(uint8_t, len, r->payload_size);
	uint8_t blank_len = rf24_has_dynamic_payload(r) ? 0 : r->payload_size - data_len;
	dbg("Reading %u bytes %u blanks\n", data_len, blank_len);

	rf24_csn(0);
	rf24_spi_write( &cmd, 1 );
	rf24_spi_read(buf, data_len);
	rf24_spi_read(blanks, blank_len);  
	rf24_csn(1);
}



#define rf24_writeout_address(r, reg, buf, len)		\
	rf24_writeout_register(r, reg, buf, len)


/**
 * Empty the receive buffer
 *
 * @param r rf24 instance to act upon
 * @return Current value of status register
 */
void rf24_flush_rx(struct rf24 *r)
{
	rf24_readout(r, FLUSH_RX, NULL, 0);
}

/**
 * Empty the transmit buffer
 * @param r rf24 instance to act upon
 * @return Current value of status register
 */
void rf24_flush_tx(struct rf24 *r)
{
	rf24_readout(r, FLUSH_TX, NULL, 0);
}


/**
 * Retrieve the current status of the chip
 *
 * @param r rf24 instance to act upon
 * @return Current value of status register
 */
uint8_t rf24_get_status(struct rf24 *r)
{
	uint8_t status;
	rf24_csn(0);
	rf24_spi_read( &status, 1 );
	rf24_csn(1);

	return status;
}


/**
 * Turn on or off the special features of the chip
 *
 * The chip has certain 'features' which are only available when the 'features'
 * are enabled.  See the datasheet for details.
 *
 * @param r rf24 instance to act upon
 */
static void rf24_toggle_features(struct rf24 *r)
{
	uint8_t data[] = { ACTIVATE, 0x73 } ;
	rf24_csn(0);
	rf24_spi_write(data, 2);
	rf24_csn(1);
}

static void write_feature(struct rf24 *r, uint8_t v)
{
	dbg("FEATURE <= %x \n", v);
	rf24_write_register(r, FEATURE, v);

	/* 
	 * If it didn't work, the features are not enabled
	 */
	
	if ( v && (!rf24_read_register(r, FEATURE)) )
	{
		/* So enable them and try again */
		dbg("Extended features aren't enabled. Enabling...\n");
		rf24_toggle_features(r);
		rf24_write_register(r, FEATURE, v);
	}


	if (v && !rf24_read_register(r, FEATURE))
		panic("Failed to enable extended features. Are they supported by chip?");


	dbg("FEATURE=%i\r\n", rf24_read_register(r, FEATURE));
}

void rf24_config(struct rf24 *r, struct rf24_config *c)
{
	/* 
	 * TODO: This method could take advantage of the 'wide_band' calculation
	 * done in setChannel() to require certain channel spacing.
	 */
	uint8_t feat;
	uint8_t dynpd;
	uint8_t setup, config;

	rf24_ce(0);

	rf24_write_register(r, RF_CH, min_t(uint8_t, c->channel, (RF24_NUM_CHANNELS - 1)));

	rf24_write_register(r, SETUP_RETR, 
			    (c->retry_timeout & 0xf)<<ARD | (c->num_retries & 0xf) << ARC);

	feat  = rf24_read_register(r, FEATURE);
	dynpd = rf24_read_register(r, DYNPD);

	r->payload_size = min_t(uint8_t, c->payload_size, RF24_MAX_PAYLOAD);

	if (c->dynamic_payloads) { 
		feat |= (1<<EN_DPL);
		r->flags |= RF24_DYNAMIC_PAYLOAD;
		dynpd|=(1<<DPL_P5) | (1<<DPL_P4) | (1<<DPL_P3) | (1<<DPL_P2) | (1<<DPL_P1) | (1<<DPL_P0);
	} else { 
		feat &= ~(1<<EN_DPL);
		r->flags &= ~(RF24_DYNAMIC_PAYLOAD);
		dynpd&=~((1<<DPL_P5) | (1<<DPL_P4) | (1<<DPL_P3) | (1<<DPL_P2) | (1<<DPL_P1) | (1<<DPL_P0));
	}

	if (c->ack_payloads) {
		r->flags |= RF24_HAVE_ACK_PAYLOADS;
		feat     |= (1<<EN_ACK_PAY) | (1<<EN_DPL);
		dynpd    |= (1 << DPL_P1) | (1 << DPL_P0);
	} else {
		r->flags &= ~RF24_HAVE_ACK_PAYLOADS;
		feat     &= ~(1<<EN_ACK_PAY);
	}
	
	write_feature(r, feat); 
	rf24_write_register(r, DYNPD, dynpd);

	setup = rf24_read_register(r, RF_SETUP) ;
	/* Data rate and PA level */
	/* HIGH and LOW '00' is 1Mbs - our default */
	r->flags |= RF24_WIDE_BAND;
	setup &= ~((1<<RF_DR_LOW) | (1<<RF_DR_HIGH)) ;
	if( c->rate == RF24_250KBPS )
	{
		/* Must set the RF_DR_LOW to 1; RF_DR_HIGH (used to be RF_DR) is already 0
		 * Making it '10'.
		 */
		r->flags &= ~(RF24_WIDE_BAND);
		setup |= (1<< RF_DR_LOW ) ;
	} else if ( c->rate == RF24_2MBPS )
	{
		setup |= (1<<RF_DR_HIGH);
	}

	/* PA Level */
	setup &= ~((1<<RF_PWR_LOW) | (1<<RF_PWR_HIGH)) ;
	
	/* switch uses RAM (evil!) */
	if ( c->pa == RF24_PA_MAX )
	{
		setup |= ((1<<RF_PWR_LOW) | (1<<RF_PWR_HIGH)) ;
	}
	else if ( c->pa == RF24_PA_HIGH )
	{
		setup |= (1<<RF_PWR_HIGH) ;
	}
	else if ( c->pa == RF24_PA_LOW )
	{
		setup |= (1<<RF_PWR_LOW);
	}
	else if ( c->pa == RF24_PA_ERROR )
	{
		/* On error, go to maximum PA */
		setup |= ((1<<RF_PWR_LOW) | (1<<RF_PWR_HIGH)) ;
	}

	rf24_write_register(r, RF_SETUP, setup);
	printk("setup <= %x\n", setup);
		
	/* CRC */
	config = rf24_read_register(r, CONFIG) & 
		~( (1<<CRCO) | (1<<EN_CRC)) ;
  
	/* switch uses RAM (evil!) */
	if ( c->crclen == RF24_CRC_8 )
	{
		config |= (1<<EN_CRC);
	}
	else
	{
		config |= (1<<EN_CRC);
		config |= (1<< CRCO );
	}
	printk("config <= %x\n", config);
	rf24_write_register( r, CONFIG, config ) ;
	
	
	/* per-pipe auto-ack */
	rf24_write_register(r, EN_AA, c->pipe_auto_ack & 0x3F );
	/* Finally, flush fifos */
	rf24_flush_rx(r);
	rf24_flush_tx(r);
}

void rf24_init(struct rf24 *r)
{
	int i;
	r->flags = RF24_WIDE_BAND;
	r->payload_size = 32;
	for (i=0; i< 5; i++)
		r->pipe0_reading_address[i] = 0;
	r->ack_payload_length = 0;

	rf24_ce(0);
	/*
	 * Must allow the radio time to settle else configuration bits will not necessarily stick.
	 * This is actually only required following power up but some settling time also appears to
	 * be required after resets too. For full coverage, we'll always assume the worst.
	 * Enabling 16b CRC is by far the most obvious case if the wrong timing is used - or skipped.
	 * Technically we require 4.5ms + 14us as a worst case. We'll just call it 5ms for good measure.
	 * WARNING: Delay is based on P-variant whereby non-P *may* require different timing.
	 *
	 */
	delay_ms(5);
}


/**
 * Start listening on the pipes opened for reading.
 *
 * Be sure to call rf24_open_reading_pipe() first.  Do not call rf24_write() while
 * in this mode, without first calling rf24_stop_listening().  Call
 * rf24_is_available() to check for incoming traffic, and rf24_read() to get it.
 *
 * @param r rf24 instance to act upon
 *
 */
void rf24_start_listening(struct rf24 *r)
{
	rf24_write_register(r, CONFIG,
			    rf24_read_register(r, CONFIG) | (1<<PWR_UP) | (1<<PRIM_RX));
	rf24_write_register(r, STATUS, (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT) );

	/* Write the pipe0 address */
	rf24_writeout_address(r, RX_ADDR_P0, r->pipe0_reading_address, 5);

	/* Go! */
	rf24_ce(1);

	/* wait for the radio to come up (130us actually only needed) */
	delay_us(130);
}

/**
 * Stop listening for incoming messages
 *
 * Do this before calling rf24_write().
 *
 * @param r rf24 instance to act upon
 */
void rf24_stop_listening(struct rf24 *r)
{
	uint8_t tmp;
	rf24_ce(0);

	tmp = rf24_read_register(r, CONFIG);
	rf24_write_register(r, CONFIG, ( tmp ) & ~(1<<PRIM_RX));

	rf24_writeout_address(r, RX_ADDR_P0, r->pipe0_writing_address, 5);
	rf24_writeout_address(r, TX_ADDR,    r->pipe0_writing_address, 5);
}


/**
 * Write a single payload to the open writing pipe. 
 *
 * Be sure to call rf24_open_writing_pipe() first to set the destination
 * of where to write to.
 *
 * This blocks until the message is successfully acknowledged by
 * the receiver or the timeout is reached. 
 *
 * If the previous call to rf24_write() failed, be sure either 
 * call rf24_flush_tx() to remove the failed payload from the TX FIFO
 * of call rf24_write() again with buf set to NULL. 
 * Calling rf24_write with NULL buffer and no failed payload in TX FIFO
 * return 0 immediately.
 *
 * The maximum size of data written is the fixed payload size, see
 * rf24_get_payload_size().  However, you can write less, and the remainder
 * will just be filled with zeroes.
 *
 * @param r rf24 instance to act upon
 * @param buf Pointer to the data to be sent. 
 * @param len Number of bytes to be sent
 * @return 0 if the payload was delivered successfully.
 *         -EIO if you tried to do a retransmition with TX fifo empty
 *         -ENODEV if the target device didn't ack us 
 */

int rf24_write(struct rf24 *r, const void* buf, uint8_t len )
{
	int ret;
	uint8_t tx_ok, tx_fail, ack_payload_available;
	uint8_t status = 0;

	
	/* Begin the write */
	ret = rf24_start_write(r, buf, len);
	if (ret < 0)
		return -EIO;
	/* 
	 * At this point we could return from a non-blocking write, and then call
	 * the rest after an interrupt
	 */

	do
	{      
		status = rf24_get_status(r);
	}
	while((!(status & ((1<<TX_DS) | (1<<MAX_RT))))) ;

	/* The part above is what you could recreate with your own interrupt handler,
	 * and then call this when you got an interrupt
	 * Call this when you get an interrupt
	 * The status tells us three things
	 *   -> The send was successful (TX_DS)
	 *   -> The send failed, too many retries (MAX_RT)
         *   -> There is an ack packet waiting (RX_DR)
	 */

	/* if we have ack payloads - (Really?)!  */
	
	if (r->flags & RF24_HAVE_ACK_PAYLOADS) {
		int i=2;
		while (!(status & (1<<RX_DR)) && i--) {
			status = rf24_get_status(r);
			delay_ms(1);
		}
	}
	
	rf24_what_happened(r, &tx_ok, &tx_fail, &ack_payload_available);
	
	dbg("tx_ok: %d tx_fail: %d ack_avail: %d\n", 
	    tx_ok, tx_fail, ack_payload_available);
	
	ret = tx_ok;

	dbg("tx result: %s\n", ret ? "OK" : "Fail");
	
	/* Handle the ack packet */
	if ( ack_payload_available )
	{
		r->ack_payload_length = rf24_get_dynamic_payload_size(r);
		r->flags |= RF24_ACK_PAYLOAD_AVAIL;
		dbg("got %d bytes of ack length\n", r->ack_payload_length);
		
	}

	if (tx_ok) 
		return 0;

	return -ENODEV;
}

/**
 * Test whether there are bytes available to be read
 *
 * Use this to discover on which pipe the message
 * arrived.
 *
 * @param r rf24 instance to act upon
 * @param[out] pipe_num Which pipe has the payload available
 * @return non-null if there is a payload available, 0 if none is
 */
int rf24_available(struct rf24 *r, uint8_t* pipe_num)
{
	uint8_t status = rf24_get_status(r);
	status = (status >> RX_P_NO ) & BIN(111);
	
	if (status != BIN(111)) { 
		if (pipe_num)
			*pipe_num = status;
		
		/* Clear the status bit */
		
		/* 
		 * ??? Should this REALLY be cleared now?  Or wait until we
		 * actually READ the payload?
		 */

		rf24_write_register(r, STATUS, (1<<RX_DR) );
	  
		/* Handle ack payload receipt */
		if ( status & (1<<TX_DS) )
		{
			rf24_write_register(r, STATUS, (1<<TX_DS));
		}
	}
	
	return (status != BIN(111));
}

/**
 * Read the payload
 *
 * Return the last payload received
 *
 * The size of data read is the fixed payload size, see rf24_get_payload_size()
 *
 * @note I specifically chose 'void*' as a data type to make it easier
 * for beginners to use.  No casting needed.
 *
 * @param r rf24 instance to act upon
 * @param buf Pointer to a buffer where the data should be written
 * @param len Maximum number of bytes to read into the buffer
 * @return Non-null if the payload was delivered successfully 0 if not
 */
int rf24_read(struct rf24 *r, void* buf, uint8_t len )
{
	/* Fetch the payload */
	rf24_read_payload(r, buf, len );

	/* was this the last of the data available? */
	return rf24_read_register(r, FIFO_STATUS) & (1<<RX_EMPTY);	
}

/**
 * Open a pipe for writing
 *
 * Only one pipe can be open at once, but you can change the pipe
 * you'll listen to.  Do not call this while actively listening.
 * Remember to rf24_stop_listening() first.
 *
 * Addresses are 40-bit hex values, e.g.:
 *
 *
 * @param r rf24 instance to act upon
 * @param address Pointer to 5 byte char[] storing the 40-bit address of the 
 * pipe to open.  This can be any value whatsoever, as long as you are 
 * the only one writing to it and only one other radio is listening to it.  
 * Coordinate these pipe addresses amongst nodes on the network.
 */
void rf24_open_writing_pipe(struct rf24 *r, uint8_t* address)
{
	const uint8_t max_payload_size = 32;
	int i;	
	rf24_writeout_address(r, RX_ADDR_P0, address, 5);
	rf24_writeout_address(r, TX_ADDR, address, 5);
	/* cache the address */
	for (i=0; i<5; i++)
		r->pipe0_writing_address[i] = address[i];

	
	rf24_write_register(r, RX_PW_P0, min_t(uint8_t, r->payload_size, max_payload_size));
}

/**
 * Open a pipe for reading
 *
 * Up to 6 pipes can be open for reading at once.  Open all the
 * reading pipes, and then call rf24_start_listening().
 *
 * @see rf24_open_writing_pipe
 *
 * @warning Pipes 1-5 should share the first 32 bits.
 * Only the least significant byte should be unique
 *
 * @warning Pipe 0 is also used by the writing pipe.  So if you open
 * pipe 0 for reading, and then rf24_start_listening(), it will overwrite the
 * writing pipe.  Ergo, do an rf24_open_writing_pipe() again before rf24_write().
 *
 * @todo Enforce the restriction that pipes 1-5 must share the top 32 bits
 *
 * @param r rf24 instance to act upon
 * @param number Which pipe# to open, 0-5.
 * @param address The 40-bit address of the pipe to open.
 */
void rf24_open_reading_pipe(struct rf24 *r, uint8_t child, uint8_t *address)
{
	/* If this is pipe 0, cache the address.  This is needed because
	 * openWritingPipe() will overwrite the pipe 0 address, so
	 * startListening() will have to restore it.
	 */
	int i;
	/*
	 * Put these on stack, since avr's progmem is unportable
	 * So this will be lesser evil than placing it .data
	 * -- Andrew
	 */  
	const uint8_t child_pipe[] =
		{
			RX_ADDR_P0, RX_ADDR_P1, RX_ADDR_P2, RX_ADDR_P3, RX_ADDR_P4, RX_ADDR_P5
		};
	const uint8_t child_payload_size[] =
		{
			RX_PW_P0, RX_PW_P1, RX_PW_P2, RX_PW_P3, RX_PW_P4, RX_PW_P5
		};
	const uint8_t child_pipe_enable[] =
		{
			1<<ERX_P0, 1<<ERX_P1, 1<<ERX_P2, 1<<ERX_P3, 1<<ERX_P4, 1<<ERX_P5
		};

	if (child == 0)
		for (i=0; i<5; i++)
			r->pipe0_reading_address[i] = address[i];

	if (child <= 6)
	{
		/* For pipes 2-5, only write the LSB */
		if ( child < 2 )
			rf24_writeout_address(r, child_pipe[child], address, 5);
		else
			rf24_writeout_register(r, child_pipe[child], address, 1);
		
		rf24_write_register(r, child_payload_size[child], r->payload_size);
		
		/* Note it would be more efficient to set all of the bits for all open
		 * pipes at once.  However, I thought it would make the calling code
		 * more simple to do it this way.
		 */
		rf24_write_register(r, 
				    EN_RXADDR, 
				    rf24_read_register(r, EN_RXADDR) | 
				    child_pipe_enable[child]
			);
	}
}

/**
 * Get Dynamic Payload Size
 *
 * For dynamic payloads, this pulls the size of the payload off
 * the chip
 *
 * @param r rf24 instance to act upon
 * @return Payload length of last-received dynamic payload
 */
uint8_t rf24_get_dynamic_payload_size(struct rf24 *r)
{
	uint8_t result = 0;

	rf24_readout(r, R_RX_PL_WID, &result, 1);
	if (result > 32)
	{
		dbg("Junk received, dropping\n");
		rf24_flush_rx(r);
		return 0;
	}
	return result;
}
 





/**
 * Enter low-power mode
 *
 * To return to normal power mode, either rf24_write() some data or
 * startListening, or rf24_power_up().
 * @param r rf24 instance to act upon
 */
void rf24_power_down(struct rf24 *r)
{
	rf24_write_register(r, CONFIG, 
			    rf24_read_register(r, CONFIG) & ~(1<<PWR_UP));
}

/**
 * Leave low-power mode - making radio more responsive
 *
 * To return to low power mode, call rf24_power_down().
 * @param r rf24 instance to act upon
 */
void rf24_power_up(struct rf24 *r)
{
	uint8_t tmp = rf24_read_register(r, CONFIG);
	rf24_write_register(r, CONFIG, ( tmp | _BV(PWR_UP) ));
	if ((tmp & _BV(PWR_UP)) == 0)
		delay_us(1500);

}


/**
 * Non-blocking write to the open writing pipe. 
 *
 * Just like rf24_write(), but it returns immediately. To find out what happened
 * to the send, catch the IRQ and then call rf24_what_happened().
 * If the previous transfer failed and there's something in TX FIFO
 * You can call this function with NULL buffer to start retransmision
 *
 * If you call this function with NULL buffer and empty TX FIFO
 * -EIO is returned
 *
 * @see rf24_write()
 * @see rf24_what_happened()
 *
 * @param r rf24 instance to act upon
 * @param buf Pointer to the data to be sent
 * @param len Number of bytes to be sent
 * @return 0 if the write started, -EIO on error
 */
int rf24_start_write(struct rf24 *r, const void* buf, uint8_t len )
{
	uint8_t tmp;
	/* Send the payload */
	if (buf)
		rf24_write_payload( r, buf, len );

	tmp = rf24_read_register(r, FIFO_STATUS);
	if (tmp & _BV(TX_EMPTY))
		return -EIO; /* TX Empty? Likely a bug in app code */
	
	/* Allons! */
	rf24_ce(1);
	delay_us(15);
	rf24_ce(0);	
	return 0;
}

/**
 * Write an ack payload for the specified pipe
 *
 * The next time a message is received on @p pipe, the data in @p buf will
 * be sent back in the acknowledgement.
 *
 * @warning According to the data sheet, only three of these can be pending
 * at any time.  I have not tested this.
 *
 * @param r rf24 instance to act upon
 * @param pipe Which pipe# (typically 1-5) will get this response.
 * @param buf Pointer to data that is sent
 * @param len Length of the data to send, up to 32 bytes max.  Not affected
 * by the static payload set by setPayloadSize().
 */
void rf24_write_ack_payload(struct rf24 *r, uint8_t pipe, const void* buf, uint8_t len)
{
	const uint8_t max_payload_size = 32;
	uint8_t data_len;
	data_len = min_t(uint8_t, len, max_payload_size);
	rf24_writeout(r, (W_ACK_PAYLOAD | ( pipe & BIN(111))), buf, data_len);
}


/**
 * Call this when you get an interrupt to find out why
 *
 * Tells you what caused the interrupt, and clears the state of
 * interrupts.
 *
 * @param r rf24 instance to act upon
 * @param[out] tx_ok The send was successful (TX_DS)
 * @param[out] tx_fail The send failed, too many retries (MAX_RT)
 * @param[out] rx_ready There is a message waiting to be read (RX_DS)
 */
void rf24_what_happened(struct rf24 *r, uint8_t *tx_ok, uint8_t *tx_fail, uint8_t *rx_ready)
{
	/* Read the status & reset the status in one easy call
	 * Or is that such a good idea?
	 */
	uint8_t status = rf24_get_status(r);
	rf24_write_register(r, STATUS, 
			    (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT) );
	
	/* Report to the user what happened */
	*tx_ok = status & (1<<TX_DS);
	*tx_fail = status & (1<<MAX_RT);
	*rx_ready = status & (1<<RX_DR);
	printk("TX_OK %x TX_FAIL %x RX_READY %x STATUS %x\n", 
	       status & (1<<TX_DS), status & (1<<MAX_RT), status & (1<<RX_DR), status);
}

/**
 * Test whether there was a carrier on the line for the
 * previous listening period.
 *
 * Useful to check for interference on the current channel.
 *
 * @param r rf24 instance to act upon
 * @return true if was carrier, false if not
 */
int rf24_test_carrier(struct rf24 *r)
{
	return ( rf24_read_register(r, CD) & 1 );
}

/**
 * Test whether a signal (carrier or otherwise) greater than
 * or equal to -64dBm is present on the channel. Valid only
 * on nRF24L01P (+) hardware. On nRF24L01, use testCarrier().
 *
 * Useful to check for interference on the current channel and
 * channel hopping strategies.
 * 
 * @param r rf24 instance to act upon
 * @return true if signal => -64dBm, false if not
 */
int rf24_test_rpd(struct rf24 *r) 
{ 
	return ( rf24_read_register(r, RPD) & 1 ) ;
}

/**
 * Determine if an ack payload was received in the most recent call to
 * write().
 *
 * Call read() to retrieve the ack payload.
 *
 * @warning Calling this function clears the internal flag which indicates
 * a payload is available.  If it returns true, you must read the packet
 * out as the very next interaction with the radio, or the results are
 * undefined.
 *
 * @return True if an ack payload is available.
 */
int rf24_is_ack_payload_available(struct rf24 *r)
{
	int ret = r->flags & RF24_ACK_PAYLOAD_AVAIL;
	r->flags &= ~RF24_ACK_PAYLOAD_AVAIL;
	return ret;
}

uint8_t rf24_tx_empty(struct rf24 *r) 
{ 
	uint8_t tmp = rf24_read_register(r, FIFO_STATUS);
	return tmp & (_BV(4));
}

uint8_t rf24_tx_full(struct rf24 *r) 
{ 
	uint8_t tmp = rf24_read_register(r, FIFO_STATUS);
	return tmp & (_BV(5));
}

/** 
 * Start writing in 'bulk' mode. This mode allows fastest 
 * possible transfers with guaranteed delivery.
 * Each of the packets will be enqueued in the TX fifo and
 * sent until the receiver ACKs it. 
 * This allows fastest possible transfer rates. 
 * To wait for the last packets to fly out of the FIFO call
 * rf24_queue_sync()
 * 
 * @param r rf24 instance to act upon
 * @param buf buffer to send
 * @param len length to send
 * 
 * @return 0 if the payload has been enqueued, -EAGAIN if there's if the TX fifo is full
 */
int rf24_queue_push(struct rf24 *r, const void* buf, uint8_t len)
{
	uint8_t tmp;
	
	tmp = rf24_read_register(r, FIFO_STATUS);
	if (tmp & _BV(5)) {
		rf24_write_register(r, STATUS, (1<<MAX_RT) );
		return -EAGAIN; /* EAGAIN, we're full o' shit right now */
	}

	/* Send the payload */
	rf24_write_payload( r, buf, len );

	/* Start sending 'em out already */
	rf24_ce(1);
	return 0; /* Queued! */
}



/** 
 * Wait for the TX fifo to become empty and 
 * stop transmitting. 
 * 
 * @param r rf24 instance to act upon
 * @param timeout How long to wait for queue synchronisation ( in 250uS intervals )
 * @return 0 - timeout,  
 */
uint16_t rf24_queue_sync(struct rf24 *r, uint16_t timeout)
{
	uint8_t tmp;
	rf24_ce(1); /* If we're attempting a resync - start actual transmission */
	while (!rf24_tx_empty(r) && --timeout) { 
		rf24_what_happened(r, &tmp, &tmp, &tmp);
		delay_ms(10); /* Wait for last packet to fly out, worst-case */
	};
	rf24_what_happened(r, &tmp, &tmp, &tmp); 	/* Clean status flags */
	rf24_ce(0); /* Stop tramission */
	return timeout;
}



#if DEBUG_LEVEL > 0
void rf24_print_details(struct rf24 *r)
{
	rf24_print_status(rf24_get_status(r));
	rf24_print_address_register(r, "RX_ADDR_P0-1", RX_ADDR_P0, 2);
	rf24_print_byte_register(r, "RX_ADDR_P2-5", RX_ADDR_P2,4);
	rf24_print_address_register(r, "TX_ADDR", TX_ADDR, 1);
	
	rf24_print_byte_register(r, "RX_PW_P0-6", RX_PW_P0, 6);
	rf24_print_byte_register(r, "EN_AA", EN_AA, 1);
	rf24_print_byte_register(r, "EN_RXADDR", EN_RXADDR, 1);
	rf24_print_byte_register(r, "RF_CH", RF_CH, 1);
	rf24_print_byte_register(r, "RF_SETUP", RF_SETUP, 1);
	rf24_print_byte_register(r, "CONFIG", CONFIG, 1);
	rf24_print_byte_register(r, "SETUP_RETR", SETUP_RETR, 1);
	rf24_print_byte_register(r, "DYNPD/FEATURE", DYNPD, 2);
	
}

#else 
void rf24_print_details(struct rf24 *r)
{
	r;
	/* Do nothing, consume nothing */
}
#endif


/**
 * Decode and print the given status to stdout
 *
 * @param status Status value to print
 *
 * @warning Does nothing if stdout is not defined.  See fdevopen in stdio.h
 */
#if DEBUG_LEVEL > 0
void rf24_print_status(uint8_t status)
{
	printk("STATUS\t\t = 0x%02x RX_DR=%x TX_DS=%x MAX_RT=%x RX_P_NO=%x TX_FULL=%x\n",
	     status,
	     (status & (1 << RX_DR))   ? 1 : 0,
	     (status & (1 << TX_DS))   ? 1 : 0,
	     (status & ( 1 << MAX_RT)) ? 1 : 0,
	     ((status >> RX_P_NO) & 0x7),
	     (status & (1 << TX_FULL)) ? 1 : 0
		);
}
#else
void rf24_print_status(uint8_t status) { status; }
#endif


/**
 * Decode and print the given 'observe_tx' value to stdout
 *
 * @param r rf24 instance to act upon
 * @param value The observe_tx value to print
 *
 * @warning Does nothing if stdout is not defined.  See fdevopen in stdio.h
 */
#if DEBUG_LEVEL > 0
void rf24_print_observe_tx(struct rf24 *r, uint8_t value)
{
	printk("OBSERVE_TX=%02x: POLS_CNT=%x ARC_CNT=%x\n",
	     value,
	     (value >> PLOS_CNT) & 0xf,
	     (value >> ARC_CNT) & 0xf
		);
}
#else
void rf24_print_observe_tx(struct rf24 *r, uint8_t value) {  }
#endif



/**
 * Print the name and value of an 8-bit register to stdout
 *
 * Optionally it can print some quantity of successive
 * registers on the same line.  This is useful for printing a group
 * of related registers on one line.
 *
 * @param r rf24 instance to act upon
 * @param name Name of the register
 * @param reg Which register. Use constants from nRF24L01.h
 * @param qty How many successive registers to print
 */
#if DEBUG_LEVEL > 0
void rf24_print_byte_register(struct rf24 *r, const char* name, uint8_t reg, uint8_t qty)
{
	while (qty--) {
		printk("%s @ *0x%02x = 0x%02x\n", 
		     name, reg, rf24_read_register(r, reg));
		reg++;
	}
}
#else
void rf24_print_byte_register(struct rf24 *r, const char* name, uint8_t reg, uint8_t qty) 
{
	r;
	name; 
	reg; 
	qty;
}
#endif


/**
 * Print the name and value of a 40-bit address register to stdout
 *
 * Optionally it can print some quantity of successive
 * registers on the same line.  This is useful for printing a group
 * of related registers on one line.
 *
 * @param r rf24 instance to act upon
 * @param name Name of the register
 * @param reg Which register. Use constants from nRF24L01.h
 * @param qty How many successive registers to print
 */
#if DEBUG_LEVEL > 0
void rf24_print_address_register(struct rf24 *r, const char* name, uint8_t reg, uint8_t qty)
{
	while (qty--)
	{
		uint8_t buffer[5];
		rf24_readout_register(r, reg++, buffer, sizeof(buffer));
		printk("%s @ *%02x = %02x:%02x:%02x:%02x:%02x\n",
		     name,
		     reg,
		     buffer[4],
		     buffer[3],
		     buffer[2],
		     buffer[1],
		     buffer[0]
			);
	}

}
#else
void rf24_print_address_register(struct rf24 *r, const char* name, uint8_t reg, uint8_t qty) 
{
	r;
	name; 
	reg; 
	qty;	
}
#endif
