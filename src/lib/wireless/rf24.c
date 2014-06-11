#include <arch/antares.h>
#include <lib/RF24.h>
#include <lib/nRF24L01.h>
#include <arch/delay.h>
#include <lib/panic.h>

#define DEBUG_LEVEL CONFIG_LIB_RF24_DEBUG
#define COMPONENT "rf24"
#include <lib/printk.h>
#include <stdint.h>


/* Agressive and hacky size optimisation, 
   mostly for avr
*/

#ifdef CONFIG_LIB_RF24_SIZEOPT
/* 
 *  r->csn(0) costs us 24 bytes 
 *  rf24_csn() costs us 4 bytes 
 *  ...
 *  PROFIT!
 */

static struct rf24 *nrf;
static void rf24_csn(uint8_t state) 
{
	nrf->csn(state);
}
static void rf24_ce(uint8_t state) 
{
	nrf->ce(state);
}

static uint8_t rf24_spi_xfer(uint8_t d) 
{
	return nrf->spi_xfer(d);
}

#else
/* In case we're not that space-hungry */
#define rf24_csn(v)       r->csn(v)
#define rf24_ce(v)        r->ce(v)
#define rf24_spi_xfer(v)  r->spi_xfer(v)

#endif

/**
 * Read a chunk of data in from a register
 *
 * @param r rf24 instance to act upon
 * @param reg Which register. Use constants from nRF24L01.h
 * @param buf Where to put the data
 * @param len How many bytes of data to transfer
 * @return Current value of status register
 */
uint8_t rf24_readout_register(struct rf24 *r,
			      uint8_t reg, uint8_t* buf, uint8_t len)
{
	uint8_t status;
	rf24_csn(0);
	status = rf24_spi_xfer( R_REGISTER | ( REGISTER_MASK & reg ) );
	while ( len-- )
		*buf++ = rf24_spi_xfer(0xff);

	rf24_csn(1);
	return status;
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
	rf24_csn(0);
	rf24_spi_xfer( R_REGISTER | ( REGISTER_MASK & reg ) );
	result = rf24_spi_xfer(0xff);
	rf24_csn(1);
	return result;
}


/**
 * Write a chunk of data to a register
 *
 * @param r rf24 instance to act upon
 * @param reg Which register. Use constants from nRF24L01.h
 * @param buf Where to get the data
 * @param len How many bytes of data to transfer
 * @return Current value of status register
 */
uint8_t rf24_writeout_register(struct rf24 *r, uint8_t reg, const uint8_t* buf, uint8_t len)
{
	uint8_t status;
	rf24_csn(0);
	status = rf24_spi_xfer( W_REGISTER | ( REGISTER_MASK & reg ) );
	while ( len-- )
		rf24_spi_xfer(*buf++);
	rf24_csn(1);
	return status;
}

uint8_t rf24_writeout_address(struct rf24 *r, uint8_t reg, const uint8_t* buf, uint8_t len)
{
	uint8_t status;
	const uint8_t *addr = &buf[len];
	rf24_csn(0);
	status = rf24_spi_xfer( W_REGISTER | ( REGISTER_MASK & reg ) );
	while ( len-- )
		rf24_spi_xfer(*(--addr));
	rf24_csn(1);
	return status;
}


/**
 * Write a single byte to a register
 *
 * @param r rf24 instance to act upon
 * @param reg Which register. Use constants from nRF24L01.h
 * @param value The new value to write
 * @return Current value of status register
 */
uint8_t rf24_write_register(struct rf24 *r, uint8_t reg, uint8_t value)
{
	uint8_t status;
	trace("write_register(%02x,%02x)\n", reg, value);
	rf24_csn(0);
	status = rf24_spi_xfer( W_REGISTER | ( REGISTER_MASK & reg ) );
	rf24_spi_xfer(value);
	rf24_csn(1);
	return status;
}



/**
 * Write the transmit payload
 *
 * The size of data written is the fixed payload size, see rf24_get_payload_size()
 *
 * @param r rf24 instance to act upon
 * @param buf Where to get the data
 * @param len Number of bytes to be sent
 * @return Current value of status register
 */
uint8_t rf24_write_payload(struct rf24 *r, const void* buf, uint8_t len)
{
	uint8_t status;
	const uint8_t* current = (uint8_t*) buf;
	uint8_t data_len = min_t(uint8_t, len, r->payload_size);
	uint8_t blank_len = rf24_has_dynamic_payload(r) ? 0 : r->payload_size - data_len;
	dbg("Writing %u bytes %u blanks\n", data_len, blank_len);
	rf24_csn(0);
	status = rf24_spi_xfer( W_TX_PAYLOAD );
	while ( data_len-- )
		rf24_spi_xfer(*current++);
	while ( blank_len-- )
		rf24_spi_xfer(0);
	rf24_csn(1);
	return status;
}

/**
 * Read the receive payload
 *
 * The size of data read is the fixed payload size, see rf24_get_payload_size()
 *
 * @param r rf24 instance to act upon
 * @param buf Where to put the data
 * @param len Maximum number of bytes to read
 * @return Current value of status register
 */
uint8_t rf24_read_payload(struct rf24 *r, void* buf, uint8_t len)
{
	uint8_t status;
	uint8_t* current = (uint8_t*) buf;
	uint8_t data_len = min_t(uint8_t, len, r->payload_size);
	uint8_t blank_len = rf24_has_dynamic_payload(r) ? 0 : r->payload_size - data_len;
	dbg("Reading %u bytes %u blanks\n", data_len, blank_len);
	rf24_csn(0);
	status = rf24_spi_xfer( R_RX_PAYLOAD );
	while ( data_len-- )
		*current++ = rf24_spi_xfer(0xff);
	while ( blank_len-- )
		rf24_spi_xfer(0xff);
	rf24_csn(1);
	return status;
}

/**
 * Empty the receive buffer
 *
 * @param r rf24 instance to act upon
 * @return Current value of status register
 */
uint8_t rf24_flush_rx(struct rf24 *r)
{
	uint8_t status;

	rf24_csn(0);
	status = rf24_spi_xfer( FLUSH_RX );
	rf24_csn(1);

	return status;
}

/**
 * Empty the transmit buffer
 * @param r rf24 instance to act upon
 * @return Current value of status register
 */
uint8_t rf24_flush_tx(struct rf24 *r)
{
	uint8_t status;

	rf24_csn(0);
	status = rf24_spi_xfer( FLUSH_TX );
	rf24_csn(1);

	return status;
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
	status = rf24_spi_xfer( NOP );
	rf24_csn(1);

	return status;
}

/**
 * Decode and print the given status to stdout
 *
 * @param status Status value to print
 *
 * @warning Does nothing if stdout is not defined.  See fdevopen in stdio.h
 */
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

/**
 * Decode and print the given 'observe_tx' value to stdout
 *
 * @param r rf24 instance to act upon
 * @param value The observe_tx value to print
 *
 * @warning Does nothing if stdout is not defined.  See fdevopen in stdio.h
 */
void rf24_print_observe_tx(struct rf24 *r, uint8_t value)
{
	printk("OBSERVE_TX=%02x: POLS_CNT=%x ARC_CNT=%x\n",
	     value,
	     (value >> PLOS_CNT) & 0xf,
	     (value >> ARC_CNT) & 0xf
		);
}

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
void rf24_print_byte_register(struct rf24 *r, const char* name, uint8_t reg, uint8_t qty)
{
	while (qty--) {
		printk("%s @ *0x%02x = 0x%02x\n", 
		     name, reg, rf24_read_register(r, reg));
		reg++;
	}
}

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

/**
 * Turn on or off the special features of the chip
 *
 * The chip has certain 'features' which are only available when the 'features'
 * are enabled.  See the datasheet for details.
 *
 * @param r rf24 instance to act upon
 */
void rf24_toggle_features(struct rf24 *r)
{
	rf24_csn(0);
	rf24_spi_xfer( ACTIVATE );
	rf24_spi_xfer( 0x73 );
	rf24_csn(1);
}

/**
 * Initialise rf24 instance with default params.
 * Normally you'd want to call this prior to doing anything.
 * 
 * @param r rf24 instance to act upon
 */

void rf24_init(struct rf24 *r)
{
	int i;
	r->flags = RF24_WIDE_BAND;
	r->payload_size = 32;
	for (i=0; i< 5; i++)
		r->pipe0_reading_address[i] = 0;
	r->ack_payload_length = 0;

	/* Hacky optimisation for size */
	#ifdef CONFIG_LIB_RF24_SIZEOPT
	nrf = r; 
	#endif 

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

	/*
	 * Set 1500uS (minimum for 32B payload in ESB@250KBPS) timeouts, to make testing a little easier
	 * WARNING: If this is ever lowered, either 250KBS mode with AA is broken or maximum packet
	 * sizes must never be used. See documentation for a more complete explanation.
	 */
	rf24_write_register(r, SETUP_RETR, (BIN(0100) << ARD) | (BIN(1111) << ARC));

	/*
	 * Restore our default PA level
	 */

	rf24_set_pa_level(r, RF24_PA_MAX ) ;

	/*
	 * Determine if this is a p or non-p RF24 module and then
	 * reset our data rate back to default value. This works
	 * because a non-P variant won't allow the data rate to
	 * be set to 250Kbps.
	 *
	 */
	if( rf24_set_data_rate(r, RF24_250KBPS ) )
	{
		r->flags |= RF24_P_VARIANT;
	}

	/*
	 *  Then set the data rate to the slowest (and most reliable) speed supported by all
	 *  hardware.
	 */
	rf24_set_data_rate(r, RF24_1MBPS ) ;

	/*
	 * Initialize CRC and request 2-byte (16bit) CRC
	 */

	rf24_set_crc_length(r, RF24_CRC_16 ) ;

	/*
	 * Disable dynamic payloads, to match dynamic_payloads_enabled setting
	 */
	rf24_write_register(r, DYNPD, 0);

	/* Reset current status
	 * Notice reset and flush is the last thing we do
	 */
	rf24_write_register(r, STATUS, (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT) );

	/*
	 * Set up default configuration.  Callers can always change it later.
	 * This channel should be universally safe and not bleed over into adjacent
	 * spectrum.
	 */

	rf24_set_channel(r, 76);

	// Flush buffers
	rf24_flush_rx(r);
	rf24_flush_tx(r);
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
  rf24_ce(0);
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
 * @return 0 if the payload was delivered successfully not-null if not
 */
int rf24_write(struct rf24 *r, const void* buf, uint8_t len )
{
	int ret = -1;
	uint8_t tx_ok, tx_fail, ack_payload_available;
	uint8_t status = 0;

	
	/* Begin the write */
	rf24_start_write(r, buf, len);

	status = rf24_read_register(r, FIFO_STATUS);
	if (status & TX_EMPTY)
		return 0; /* TX Empty? Why? */

	/* 
	 * At this point we could return from a non-blocking write, and then call
	 * the rest after an interrupt
	 */

	do
	{      
		uint8_t observe_tx;
		status = rf24_readout_register(r, OBSERVE_TX, &observe_tx, 1);
	}
	while((!(status & ((1<<TX_DS) | (1<<MAX_RT))))) ;


	uint8_t s = status;

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
			uint8_t observe_tx;
			status = rf24_readout_register(r, OBSERVE_TX, &observe_tx, 1);
			delay_ms(1);
		}
	}
	
	rf24_what_happened(r, &tx_ok, &tx_fail, &ack_payload_available);
	
	if (tx_fail)
		rf24_flush_tx(r);

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
	
	return !ret;
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
	int result = !(rf24_read_register(r, FIFO_STATUS) & (1<<RX_EMPTY));;	
	if (result)
	{
		/* If the caller wants the pipe number, include that */
		if ( pipe_num )
			*pipe_num = ( status >> RX_P_NO ) & BIN(111);
		
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
	
	return result;
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
	
	rf24_writeout_address(r, RX_ADDR_P0, address, 5);
	rf24_writeout_address(r, TX_ADDR, address, 5);

	const uint8_t max_payload_size = 32;
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


/**@}*/
/**
 * @name Optional Configurators 
 *
 *  Methods you can use to get or set the configuration of the chip.
 *  None are required.  Calling rf24_init() sets up a reasonable set of
 *  defaults.
 */
/**@{*/


/**
 * Set the number and delay of retries upon failed submit
 *
 * @param r rf24 instance to act upon
 * @param delay How long to wait between each retry, in multiples of 250us,
 * max is 15.  0 means 250us, 15 means 4000us.
 * @param count How many retries before giving up, max 15
 */
void rf24_set_retries(struct rf24 *r, uint8_t delay, uint8_t count)
{
	rf24_write_register(r, SETUP_RETR, 
			    (delay & 0xf)<<ARD | (count & 0xf) << ARC);
}

/**
 * Set RF communication channel
 *
 * @param r rf24 instance to act upon
 * @param channel Which RF channel to communicate on, 0-127
 */
void rf24_set_channel(struct rf24 *r, uint8_t channel)
{
	/* 
	 * TODO: This method could take advantage of the 'wide_band' calculation
	 * done in setChannel() to require certain channel spacing.
	 */
	
	const uint8_t max_channel = 127;
	rf24_write_register(r, RF_CH, min_t(uint8_t, channel, max_channel));
}

/**
 * Set Static Payload Size
 *
 * This implementation uses a pre-stablished fixed payload size for all
 * transmissions.  If this method is never called, the driver will always
 * transmit the maximum payload size (32 bytes), no matter how much
 * was sent to write().
 *
 * @todo Implement variable-sized payloads feature
 *
 * @param r rf24 instance to act upon
 * @param size The number of bytes in the payload
 */
void rf24_set_payload_size(struct rf24 *r, uint8_t size)
{
	const uint8_t max_payload_size = 32;
	r->payload_size = min_t(uint8_t, size, max_payload_size);
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
	rf24_csn(0);
	rf24_spi_xfer( R_RX_PL_WID );
	result = rf24_spi_xfer(0xff);
	rf24_csn(1);
	if (result > 32)
	{
		dbg("Junk received, dropping\n");
		rf24_flush_rx(r);
	}
	return result;
}
 


static void write_feature(struct rf24 *r, uint8_t v)
{
	rf24_write_register(r, FEATURE, v);

	/* 
	 * If it didn't work, the features are not enabled
	 */
	
	if ( ! rf24_read_register(r, FEATURE) )
	{
		/* So enable them and try again */
		dbg("Extended features aren't enabled. Enabling...");
		rf24_toggle_features(r);
		rf24_write_register(r, FEATURE, v);
	}
	if (!rf24_read_register(r, FEATURE))
		panic("Failed to enable extended features. Are they supported by chip?");

	dbg("FEATURE=%i\r\n", rf24_read_register(r, FEATURE));

}
 
/**
 * Enable custom payloads on the acknowledge packets
 *
 * Ack payloads are a handy way to return data back to senders without
 * manually changing the radio modes on both units.
 *
 * @param r rf24 instance to act upon
 */
void rf24_enable_ack_payload(struct rf24 *r)
{
	write_feature(r, rf24_read_register(r, FEATURE) | 
		      (1<<EN_ACK_PAY) | (1<<EN_DPL) 
		);
	rf24_write_register(r, DYNPD, rf24_read_register(r, DYNPD) | (1 << DPL_P1) | (1 << DPL_P0));
	r->flags |= RF24_HAVE_ACK_PAYLOADS;
}

/**
 * Enable dynamically-sized payloads
 *
 * This way you don't always have to send large packets just to send them
 * once in a while.  This enables dynamic payloads on ALL pipes.
 *
 * @param r rf24 instance to act upon
 */
void rf24_enable_dynamic_payloads(struct rf24 *r)
{
	write_feature(r, rf24_read_register(r, FEATURE) | 
		      (1<<EN_DPL) 
		);
	/* Enable dynamic payload on all pipes
	 *
	 * Not sure the use case of only having dynamic payload on certain
	 * pipes, so the library does not support it.
	 */
	
	rf24_write_register(r, DYNPD, rf24_read_register(r, DYNPD) | 
			    (1<<DPL_P5) | (1<<DPL_P4) | (1<<DPL_P3) | (1<<DPL_P2) | (1<<DPL_P1) | (1<<DPL_P0));
	
	r->flags |= RF24_DYNAMIC_PAYLOAD;
}

/**
 * Enable or disable auto-acknowlede packets
 *
 * This is enabled by default, so it's only needed if you want to turn
 * it off for some reason.
 *
 * @param r rf24 instance to act upon
 * @param enable Whether to enable (true) or disable (false) auto-acks
 */
void rf24_set_auto_ack(struct rf24 *r, int enable)
{
	if ( enable )
		rf24_write_register(r, EN_AA, BIN(111111));
	else
		rf24_write_register(r, EN_AA, 0);
}

/**
 * Enable or disable auto-acknowlede packets on a per pipeline basis.
 *
 * AA is enabled by default, so it's only needed if you want to turn
 * it off/on for some reason on a per pipeline basis.
 *
 * @param r rf24 instance to act upon
 * @param pipe Which pipeline to modify
 * @param enable Whether to enable (true) or disable (false) auto-acks
 */
void rf24_set_pipe_auto_ack(struct rf24 *r, uint8_t pipe, int enable ) 
{
	if ( pipe <= 6 )
	{
		uint8_t en_aa = rf24_read_register(r, EN_AA ) ;
		if( enable )
		{
			en_aa |= (1<<pipe) ;
		}
		else
		{
			en_aa &= ~(1<<pipe) ;
		}
		rf24_write_register(r, EN_AA, en_aa ) ;
	}
}


/**
 * Set Power Amplifier (PA) level to one of four levels.
 * Relative mnemonics have been used to allow for future PA level
 * changes. According to 6.5 of the nRF24L01+ specification sheet,
 * they translate to: RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm,
 * RF24_PA_MED=-6dBM, and RF24_PA_HIGH=0dBm.
 *
 * @param r rf24 instance to act upon
 * @param level Desired PA level.
 */
void rf24_set_pa_level( struct rf24 *r, rf24_pa_dbm_e level ) 
{
	uint8_t setup = rf24_read_register(r, RF_SETUP) ;
	setup &= ~((1<<RF_PWR_LOW) | (1<<RF_PWR_HIGH)) ;
	
	/* switch uses RAM (evil!) */
	if ( level == RF24_PA_MAX )
	{
		setup |= ((1<<RF_PWR_LOW) | (1<<RF_PWR_HIGH)) ;
	}
	else if ( level == RF24_PA_HIGH )
	{
		setup |= (1<<RF_PWR_HIGH) ;
	}
	else if ( level == RF24_PA_LOW )
	{
		setup |= (1<<RF_PWR_LOW);
	}
	else if ( level == RF24_PA_MIN )
	{
		/* nothing */
	}
	else if ( level == RF24_PA_ERROR )
	{
		/* On error, go to maximum PA */
		setup |= ((1<<RF_PWR_LOW) | (1<<RF_PWR_HIGH)) ;
	}
	
	rf24_write_register(r, RF_SETUP, setup ) ;
}

/**
 * Fetches the current PA level.
 *
 * @param r rf24 instance to act upon
 *
 * @return Returns a value from the rf24_pa_dbm_e enum describing
 * the current PA setting. Please remember, all values represented
 * by the enum mnemonics are negative dBm. See setPALevel for
 * return value descriptions.
 */
rf24_pa_dbm_e rf24_get_pa_level( struct rf24 *r ) 
{
	rf24_pa_dbm_e result = RF24_PA_ERROR ;
	uint8_t power = rf24_read_register(r, RF_SETUP) & 
		((1<<RF_PWR_LOW) | (1<<RF_PWR_HIGH)) ;

	/* switch uses RAM (evil!) */
	if ( power == ((1<<RF_PWR_LOW) | (1<<RF_PWR_HIGH)) )
	{
		result = RF24_PA_MAX ;
	}
	else if ( power == (1<<RF_PWR_HIGH) )
	{
		result = RF24_PA_HIGH ;
	}
	else if ( power == (1<<RF_PWR_LOW) )
	{
		result = RF24_PA_LOW ;
	}
	else
	{
		result = RF24_PA_MIN ;
	}

	return result ;
}

/**
 * Set the transmission data rate
 *
 * @warning setting RF24_250KBPS will fail for non-plus units
 *
 * @param r rf24 instance to act upon
 * @param speed RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
 * @return true if the change was successful
 */
int rf24_set_data_rate(struct rf24 *r, rf24_datarate_e speed)
{
	int result = 0;
	uint8_t setup = rf24_read_register(r, RF_SETUP) ;

	/* HIGH and LOW '00' is 1Mbs - our default */
	r->flags &= ~(RF24_WIDE_BAND);
	setup &= ~((1<<RF_DR_LOW) | (1<<RF_DR_HIGH)) ;
	if( speed == RF24_250KBPS )
	{
		/* Must set the RF_DR_LOW to 1; RF_DR_HIGH (used to be RF_DR) is already 0
		 * Making it '10'.
		 */
		r->flags &= ~(RF24_WIDE_BAND);
		setup |= (1<< RF_DR_LOW ) ;
	}
	else
	{
		/* Set 2Mbs, RF_DR (RF_DR_HIGH) is set 1
		 * Making it '01'
		 */
		if ( speed == RF24_2MBPS )
		{
			r->flags |= RF24_WIDE_BAND;
			setup |= (1<<RF_DR_HIGH);
		}
		else
		{
			/* 1Mbs */
			r->flags |= RF24_WIDE_BAND;
		}
	}
	rf24_write_register(r, RF_SETUP, setup);

	/* Verify our result */
	if ( rf24_read_register(r, RF_SETUP) == setup )
	{
		result = 1;
	}
	else
	{
		r->flags &= ~(RF24_WIDE_BAND);
	}
	return result;
}
  
/**
 * Fetches the transmission data rate
 *
 * @param r rf24 instance to act upon
 * @return Returns the hardware's currently configured datarate. The value
 * is one of 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS, as defined in the
 * rf24_datarate_e enum.
 */
rf24_datarate_e rf24_get_data_rate( struct rf24 *r )
{
	rf24_datarate_e result ;
	uint8_t dr = rf24_read_register(r, RF_SETUP) & 
		((1<<RF_DR_LOW) | (1<<RF_DR_HIGH));
	
	/* switch uses RAM (evil!)
	 * Order matters in our case below
	 */
	if ( dr == (1<<RF_DR_LOW) )
	{
		/* '10' = 250KBPS */
		result = RF24_250KBPS ;
	}
	else if ( dr == (1<<RF_DR_HIGH) )
	{
		/* '01' = 2MBPS */
		result = RF24_2MBPS ;
	}
	else
	{
		/* '00' = 1MBPS */
		result = RF24_1MBPS ;
	}
	return result ;
}

/**
 * Set the CRC length
 *
 * @param r rf24 instance to act upon
 * @param length RF24_CRC_8 for 8-bit or RF24_CRC_16 for 16-bit
 */

void rf24_set_crc_length(struct rf24 *r, rf24_crclength_e length)
{
	uint8_t config = rf24_read_register(r, CONFIG) & 
		~( (1<<CRCO) | (1<<EN_CRC)) ;
  
	/* switch uses RAM (evil!) */
	if ( length == RF24_CRC_DISABLED )
	{
		/* Do nothing, we turned it off above. */ 
	}
	else if ( length == RF24_CRC_8 )
	{
		config |= (1<<EN_CRC);
	}
	else
	{
		config |= (1<<EN_CRC);
		config |= (1<< CRCO );
	}
	rf24_write_register( r, CONFIG, config ) ;
}

/**
 * Get the CRC length
 *
 * @param r rf24 instance to act upon
 * @return RF24_DISABLED if disabled or RF24_CRC_8 for 8-bit or RF24_CRC_16 for 16-bit
 */
rf24_crclength_e rf24_get_crc_length(struct rf24 *r)
{
	rf24_crclength_e result = RF24_CRC_DISABLED;
	uint8_t config = rf24_read_register(r, CONFIG) & ( (1<<CRCO) | (1<<EN_CRC)) ;

	if ( config & (1<<EN_CRC ) )
	{
		if ( config & (1<<CRCO) )
			result = RF24_CRC_16;
		else
			result = RF24_CRC_8;
	}

	return result;
}

/**
 * Disable CRC validation
 *
 * @param r rf24 instance to act upon
 */
void rf24_disable_crc( struct rf24 *r ) 
{
	uint8_t disable = rf24_read_register(r, CONFIG) & ~(1<<EN_CRC) ;
	rf24_write_register(r,  CONFIG, disable ) ;	
}

/**@}*/
/**
 * @name Advanced Operation 
 *
 *  Methods you can use to drive the chip in more advanced ways 
 */
/**@{*/

/**
 * Print a giant block of debugging information to console
 *
 * @param r rf24 instance to act upon
 * @warning Does nothing if no console active, or 
 *          CONFIG_LIB_RF24_DEBUG is other than 0
 */
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
	rf24_print_byte_register(r, "DYNPD/FEATURE", DYNPD, 2);
	
	printk("Data Rate\t = %d\n",rf24_get_data_rate(r));
	printk("Model\t\t = %d\n", rf24_is_p_variant(r));
	printk("CRC Length\t = %d\n", rf24_get_crc_length(r));
	printk("PA Power\t = %d\n", rf24_get_pa_level(r));
}
#else 
void rf24_print_details(struct rf24 *r)
{
	/* Do nothing, consume nothing */
}
#endif

/**
 * Enter low-power mode
 *
 * To return to normal power mode, either rf24_write() some data or
 * startListening, or rf24_power_pp().
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
		rf24_write_register(r, CONFIG, 
			    rf24_read_register(r, CONFIG) | (1<<PWR_UP));
}


/**
 * Non-blocking write to the open writing pipe. 
 *
 * Just like rf24_write(), but it returns immediately. To find out what happened
 * to the send, catch the IRQ and then call rf24_what_happened().
 * If the previous transfer failed and there's something in TX FIFO
 * You can call this function with NULL buffer to retransmit
 *
 * @see rf24_write()
 * @see rf24_what_happened()
 *
 * @param r rf24 instance to act upon
 * @param buf Pointer to the data to be sent
 * @param len Number of bytes to be sent
 * @return True if the payload was delivered successfully false if not
 */
void rf24_start_write(struct rf24 *r, const void* buf, uint8_t len )
{

	uint8_t old_config = rf24_read_register(r, CONFIG);
	rf24_write_register(r, CONFIG, ( old_config | (1<<PWR_UP) ) & ~(1<<PRIM_RX));
	if ((old_config & _BV(PWR_UP)) == 0)
		delay_us(1500);
	
	/* Send the payload */
	if (buf)
		rf24_write_payload( r, buf, len );
	
	/* Allons! */
	rf24_ce(1);
	delay_us(15);
	rf24_ce(0);	
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
	const uint8_t* current = (const uint8_t*)(buf);
	
	rf24_csn(0);
	rf24_spi_xfer( W_ACK_PAYLOAD | ( pipe & BIN(111) ) );
	const uint8_t max_payload_size = 32;
	uint8_t data_len = min_t(uint8_t, len, max_payload_size);
	while ( data_len-- )
		rf24_spi_xfer(*current++);	
	rf24_csn(1);
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
	uint8_t status = rf24_write_register(r, STATUS, 
					     (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT) );
	
	/* Report to the user what happened */
	*tx_ok = status & (1<<TX_DS);
	*tx_fail = status & (1<<MAX_RT);
	*rx_ready = status & (1<<RX_DR);
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
inline int rf24_is_ack_payload_available(struct rf24 *r)
{
	int ret = r->flags & RF24_ACK_PAYLOAD_AVAIL;
	r->flags &= ~RF24_ACK_PAYLOAD_AVAIL;
	return ret;
}

/** 
 * Start writing in 'stream' mode. This mode allows fastest 
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
	uint8_t tmp = rf24_read_register(r, CONFIG);
	rf24_write_register(r, CONFIG, ( tmp | (1<<PWR_UP) ) & ~(1<<PRIM_RX));
	if ((tmp & _BV(PWR_UP)) == 0)
		delay_us(1500);
	
	tmp = rf24_read_register(r, FIFO_STATUS);
	if (tmp & TX_FULL)
		return -1; /* EAGAIN, we're full o' shit right now */

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
 */
void rf24_queue_sync(struct rf24 *r)
{
	uint8_t tmp;
	do { 
		tmp = rf24_read_register(r, FIFO_STATUS);
	} while (!(tmp & TX_EMPTY));
	rf24_ce(0);
}
