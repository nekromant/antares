/*
  Copyright (C) 2013 Andrew Andrianov <andrew@ncrmnt.org>
  Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  version 2 as published by the Free Software Foundation.
*/

#ifndef RF24_H
#define RF24_H
#include <stdint.h>

/**
 * Driver for nRF24L01(+) 2.4GHz Wireless Transceiver
 */


/**
 * Power Amplifier level.
 *
 * For use with setPALevel()
 */
typedef enum { RF24_PA_MIN = 0,RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX, RF24_PA_ERROR } rf24_pa_dbm_e ;

/**
 * Data rate.  How fast data moves through the air.
 *
 * For use with setDataRate()
 */
typedef enum { RF24_1MBPS = 0, RF24_2MBPS, RF24_250KBPS } rf24_datarate_e;

/**
 * CRC Length.  How big (if any) of a CRC is included.
 *
 * For use with setCRCLength()
 */
typedef enum { RF24_CRC_DISABLED = 0, RF24_CRC_8, RF24_CRC_16 } rf24_crclength_e;


#define RF24_NUM_CHANNELS 128 
#define RF24_MAX_PAYLOAD  32

#define RF24_WIDE_BAND           (1<<0)
#define RF24_P_VARIANT           (1<<1)
#define RF24_ACK_PAYLOAD_AVAIL   (1<<2)
#define RF24_DYNAMIC_PAYLOAD     (1<<3)
#define RF24_HAVE_ACK_PAYLOADS   (1<<4)



#define rf24_is_wideband(rf24)         ((rf24)->flags & RF24_WIDE_BAND)
#define rf24_is_p_variant(rf24)        ((rf24)->flags & RF24_P_VARIANT)

#define rf24_has_dynamic_payload(rf24) ((rf24)->flags & RF24_DYNAMIC_PAYLOAD)
struct rf24 {
	void    (*csn)(int level);
	void    (*ce)(int level);
	void    (*spi_set_speed)(int khz);
	uint8_t (*spi_xfer)(uint8_t dat);
	/* private data below */
	uint8_t flags;
	uint8_t payload_size;
	uint8_t ack_payload_length;
	uint8_t pipe0_reading_address[5];
	uint8_t pipe0_writing_address[5];
	uint8_t nretr; 
	uint8_t rtimeout;
};

struct rf24_sweeper {
	struct rf24* r; 
	uint8_t values[RF24_NUM_CHANNELS];
};


void rf24_init(struct rf24 *r); 

uint8_t rf24_readout_register(struct rf24 *r, uint8_t reg, uint8_t* buf, uint8_t len);
uint8_t rf24_read_register(struct rf24 *r, uint8_t reg);
uint8_t rf24_writeout_register(struct rf24 *r, uint8_t reg, const uint8_t* buf, uint8_t len);
uint8_t rf24_write_register(struct rf24 *r, uint8_t reg, uint8_t value);
uint8_t rf24_write_payload(struct rf24 *r, const void* buf, uint8_t len);
uint8_t rf24_read_payload(struct rf24 *r, void* buf, uint8_t len);
uint8_t rf24_flush_rx(struct rf24 *r);
uint8_t rf24_flush_tx(struct rf24 *r);
uint8_t rf24_get_status(struct rf24 *r);



void rf24_print_status(uint8_t status);
void rf24_print_observe_tx(struct rf24 *r, uint8_t value);
void rf24_print_byte_register(struct rf24 *r, const char* name, uint8_t reg, uint8_t qty);
void rf24_print_address_register(struct rf24 *r, const char* name, uint8_t reg, uint8_t qty);

void rf24_toggle_features(struct rf24 *r);
void rf24_start_listening(struct rf24 *r);
void rf24_stop_listening(struct rf24 *r);
int rf24_write(struct rf24 *r, const void* buf, uint8_t len );
int rf24_read(struct rf24 *r, void* buf, uint8_t len );
void rf24_open_writing_pipe(struct rf24 *r, uint8_t *address);
void rf24_open_reading_pipe(struct rf24 *r, uint8_t number, uint8_t *address);
void rf24_set_retries(struct rf24 *r, uint8_t delay, uint8_t count);
void rf24_set_channel(struct rf24 *r, uint8_t channel);
void rf24_set_payload_size(struct rf24 *r, uint8_t size);

/**
 * Get Static Payload Size
 *
 * @see rf24_set_payload_size()
 *
 * @param r rf24 instance to act upon
 * @return The number of bytes in the payload
 */
#define rf24_get_payload_size(r) ((r)->payload_size)

uint8_t rf24_get_dynamic_payload_size(struct rf24 *r);
void rf24_enable_ack_payload(struct rf24 *r);
void rf24_enable_dynamic_payloads(struct rf24 *r);
void rf24_set_auto_ack(struct rf24 *r, int enable);
void rf24_set_pipe_auto_ack(struct rf24 *r, uint8_t pipe, int enable ) ;
void rf24_set_pa_level( struct rf24 *r, rf24_pa_dbm_e level ) ;
rf24_pa_dbm_e rf24_get_pa_level( struct rf24 *r ) ;
int rf24_set_data_rate(struct rf24 *r, rf24_datarate_e speed);
rf24_datarate_e rf24_get_data_rate( struct rf24 *r ) ;
void rf24_set_crc_length(struct rf24 *r, rf24_crclength_e length);
rf24_crclength_e rf24_get_crc_length(struct rf24 *r);
void rf24_disable_crc( struct rf24 *r ) ;
void rf24_print_details(struct rf24 *r);
void rf24_power_down(struct rf24 *r);
void rf24_power_up(struct rf24 *r) ;
int rf24_available(struct rf24 *r, uint8_t* pipe_num);
int rf24_start_write(struct rf24 *r, const void* buf, uint8_t len );
void rf24_write_ack_payload(struct rf24 *r, uint8_t pipe, const void* buf, uint8_t len);
int rf24_is_ack_payload_available(struct rf24 *r);
void rf24_what_happened(struct rf24 *r, uint8_t *tx_ok, uint8_t *tx_fail, uint8_t *rx_ready);
int rf24_test_carrier(struct rf24 *r);
int rf24_test_rpd(struct rf24 *r) ;


int rf24_queue_push(struct rf24 *r, const void* buf, uint8_t len);
uint16_t rf24_queue_sync(struct rf24 *r, uint16_t timeout);

void rf24_sweeper_init(struct rf24_sweeper *s, struct rf24 *r);
void rf24_sweep(struct rf24_sweeper *s, int loops);
void rf24_sweep_dump_results(struct rf24_sweeper *s);
void rf24_sweep_dump_header();


#endif
