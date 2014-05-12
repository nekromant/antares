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
	RQ_GET_EVT
};

#define IOBUFLEN 32
#define EVT_BUFLEN 128;
static unsigned char msg[IOBUFLEN]; 


#include <lib/circ_buf.h>


void urpc_evt_enqueue(urpc_id_t id, char *data, urpc_size_t sz)
{
	
}

static void evt_fetch(char *dest, urpc_size_t destsz)
{
	
}

/* Handle responses and events */
static void  vusb_action(urpc_id_t id, char *data, urpc_size_t sz)
{
	
}

static struct urpc_object * objs_data[10];
static struct urpc urpc_vusb =  {						
		.num_objects = 0,					
		.max_objects = 10,					
		.objects = objs_data		
};


struct urpc *g_urpc_vusb = &urpc_vusb; /* Global pointer */



static uint8_t pos;
static uint8_t rq_len;

static urpc_id_t to_call; /* Which one to call */

static int pack_str(char* dst, char* src)
{
	int ret = 0;
	if (src) { 
		strcpy(dst, src);
		ret += strlen(src) + 1;	
	} else { 
		*dst = 0; 
		ret = 1; 
	}
	return ret; 
}
	  

uchar   usbFunctionSetup(uchar data[8])
{
        usbRequest_t    *rq = (void *)data;
	int ret=0;
        if (rq->bRequest == RQ_GET_INFO_TOKEN) /* This nukes the event queue */
        {
		ret = snprintf_P((char *) msg, IOBUFLEN, 
				 PSTR("endianness:%s urpc_size_t=%d, urpc_id_t=%d, iobuf=%d"),
				 "l", sizeof(urpc_size_t), sizeof(urpc_id_t), IOBUFLEN) + 1;
		/* TODO: Nuke the evt queue */
	} else if (rq->bRequest == RQ_GET_NUM_OBJECTS) {
		ret = snprintf_P((char *) msg, IOBUFLEN, PSTR("%u"), g_urpc_vusb->num_objects) + 1;
		ANTARES_ATOMIC_BLOCK() { 
			g_urpc_vusb->evt_head = 0; 
			g_urpc_vusb->evt_tail = 0;
		}
	} else if (rq->bRequest == RQ_GET_OBJECTINFO) {
		if (rq->wValue.word > g_urpc_vusb->num_objects - 1)
			return 0;
		char * tmp = msg;
		if (g_urpc_vusb->objects[rq->wValue.word]->method) 
			msg[ret++] = 'm'; /* method */
		else
			msg[ret++] = 'e'; /* event */
		
		ret += pack_str(&msg[ret], g_urpc_vusb->objects[rq->wValue.word]->name);
		ret += pack_str(&msg[ret], g_urpc_vusb->objects[rq->wValue.word]->argfmt);
		ret += pack_str(&msg[ret], g_urpc_vusb->objects[rq->wValue.word]->responsefmt);

	} else if (rq->bRequest == RQ_CALL) {
		to_call = rq->wValue.word; 
		rq_len = rq->wLength.word;
		pos = 0;
		if (rq_len==0) {
			urpc_call(g_urpc_vusb, to_call, NULL, 0); 
			return 0; 
		}
		return USB_NO_MSG;
		/* Pass on to write */
	} else if (rq->bRequest == RQ_GET_EVT)
	{
		if (0==CIRC_CNT(urpc_vusb.evt_head, 
				urpc_vusb.evt_tail, 
				CONFIG_URPC_EBLEN))
			return 0;
		urpc_id_t *id;
		urpc_size_t *sz; 
		urpc_evt_read(g_urpc_vusb, msg, sizeof(urpc_id_t) + sizeof(urpc_size_t));
		id = &msg;
		sz = &msg[sizeof(urpc_id_t)];
		urpc_evt_read(g_urpc_vusb, 
			      &msg[sizeof(urpc_id_t) + sizeof(urpc_size_t)], 
			      *sz
			);
		return *sz + sizeof(urpc_id_t) + sizeof(urpc_size_t);
	}
	usbMsgPtr = msg;
	return ret;
}

uchar usbFunctionWrite(uchar *data, uchar len)
{
	memcpy(&msg[pos], data, len);
        pos+=len;
        if (pos != rq_len) 
		return 0;
	/* Do the actual call */
	
	urpc_call(g_urpc_vusb, to_call, msg, rq_len);

	return 1;
}


static unsigned char periodic; 
ANTARES_APP(vusb_thread)
{
	usbPoll();
	if (usbInterruptIsReady()) {
		periodic =  CIRC_CNT(urpc_vusb.evt_head, 
				       urpc_vusb.evt_tail, 
				       CONFIG_URPC_EBLEN);
		if (periodic) 
			usbSetInterrupt(&periodic, 1);
	}
}
