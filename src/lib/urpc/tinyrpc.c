#include <lib/urpc.h>
#include <string.h>

#define STATE_DISCOVERY  1 

static unsigned char state;
static unsigned char objid;
static const char urpc_name[] = "elvelyn";

/* TODO: Optional packet queue */
#ifndef CONFIG_URPC_ISR_CONTEXT
static struct urpc_packet *packet;
#endif

static void process_packet(struct urpc_packet *pck) {
	int id = (int) pck->id;
	if (urpc_exports[id].method)
		urpc_exports[id].method(&pck->data);
}

void urpc_loop() {
#ifndef CONFIG_URPC_ISR_CONTEXT
	if (packet)
		process_packet(packet);
#endif
	if (state == STATE_DISCOVERY) {
		urpc_tx_object(&urpc_exports[objid++]);
		if (!urpc_exports[objid].flags)
		{
			urpc_tx_data(0,0,0);
			state=0;
		}
	}	
}

void urpc_handle_incoming(struct urpc_packet* pck) {
#ifdef CONFIG_URPC_ISR_CONTEXT
	process_packet(pck);
#else
	if (!packet)
		packet = pck;
	/* We discard a packet if we're busy */
#endif
}

void urpc_discovery() {
	struct urpc_object *obj;
	obj = &urpc_exports[0];
	urpc_tx_data(0,urpc_name,strlen(urpc_name)+1);
	state = STATE_DISCOVERY;
	objid=0;

}


