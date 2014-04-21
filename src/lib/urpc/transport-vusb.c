#include <arch/antares.h>
#include <avr/boot.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <generated/usbconfig.h>
#include <arch/vusb/usbportability.h>
#include <arch/vusb/usbdrv.h>
#include <arch/delay.h>
#include <lib/urpc.h>
#include <stdio.h>
#include <string.h>

enum {
	RQ_GET_INFO_TOKEN=0,
	RQ_GET_NUM_OBJECTS,
	RQ_GET_OBJECTINFO,
	RQ_CALL,
	RQ_POLL
};

struct vusb_urpc_call {
	urpc_id_t id;
	char data[];
} __attribute__((packed));



URPC_DECLARE_INSTANCE(urpc_vusb, NULL, 10);

struct urpc *g_urpc_vusb = &urpc_vusb; /* Global pointer */
#define IOBUFLEN 128
static unsigned char msg[IOBUFLEN]; 
static uint8_t pos;
static uint8_t rq_len;

uchar   usbFunctionSetup(uchar data[8])
{
        usbRequest_t    *rq = (void *)data;
        if (rq->bRequest == RQ_GET_INFO_TOKEN) /* This nukes the event queue */
        {
		sprintf_P((char *) msg, PSTR("endianness:%s urpc_size_t=%d, urpc_id_t=%d"),
			  "l", sizeof(urpc_size_t), sizeof(urpc_id_t));
	} else if (rq->bRequest == RQ_GET_NUM_OBJECTS) {
		sprintf_P((char *) msg, PSTR("%u"), g_urpc_vusb->num_objects);
	} else if (rq->bRequest == RQ_GET_OBJECTINFO) {
		sprintf_P((char *) msg, PSTR("%u"), g_urpc_vusb->num_objects);
	} else if (rq->bRequest == RQ_CALL) {
		rq_len = rq->wLength.word;
		pos = 0;
		return USB_NO_MSG;
		/* Pass on to write */
	} else if (rq->bRequest == RQ_POLL)
	{
		
	}
	usbMsgPtr = msg;
	return strlen((char*) msg)+1;
}

uchar usbFunctionWrite(uchar *data, uchar len)
{
	memcpy(&msg[pos], data, len);
        pos+=len;
        if (pos != rq_len) 
		return 0;
	/* Do the call */
	struct vusb_urpc_call *c = (struct vusb_urpc_call *) msg;
	urpc_call(g_urpc_vusb, c->id, c->data, rq_len);
	return 1;
}


