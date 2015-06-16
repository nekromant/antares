#include <arch/antares.h>
#include <lib/RF24.h>
#include <lib/nRF24L01.h>
#include <arch/delay.h>
#include <lib/panic.h>
#include <string.h>

#define DEBUG_LEVEL CONFIG_LIB_RF24_DEBUG
#define COMPONENT "rf24-pmgr"
#include <lib/printk.h>
#include <stdint.h>


void rf24pmgr_register_callback(struct rf24 *radio, uint8_t pipe, 
				void (*callback)(char* data, uint8_t len, void *arg), void *arg)
{
	if (pipe > 5)
		return; 
	radio->pmgr_callbacks[pipe].cb  = callback;
	radio->pmgr_callbacks[pipe].arg = arg;
}

void rf24pmgr_dispatch(struct rf24 *radio) 
{
	char packet[RF24_MAX_PAYLOAD];
	uint8_t pipe;
	while (rf24_available(g_radio, &pipe)) {
		uint8_t len = rf24_get_dynamic_payload_size(radio);
		rf24_read(radio, packet, len);
		if (radio->pmgr_callbacks[pipe].cb) 
			radio->pmgr_callbacks[pipe].cb(packet, len, radio->pmgr_callbacks[pipe].arg);
	}
}
