/* 
 * Very simple and limited urpc transport
 * over conventional serial. Relies on:
 * sizeof(urpc_id_t)   == 1
 * sizeof(urpc_size_t) == 1
 * Making it support for different lengths would
 * increase code size, so let it be.
 * Packet with empty payload triggers discovery
 * You need to supply a 'putchar' and call 'gotchar'
 * Whenever a byte is received
 *
 */

#if 0
#include <string.h>
#include <lib/urpc.h>

#define SYNC '['
#define STOP ']'
#define ACK  'A'
#define NACK 'N'

static unsigned char buf[16];
static int ptr=0;
static char state=0;
static char sz=0;
static unsigned char csum;

void putchar(char data);
static void putdata(const char* data, int len);

static void csum_add(unsigned char* data, int len) 
{
	while (len--) {
		csum+=data[len];
	}
}


static const char mode[] = URPC_MODE_TAG;


void serialdiscovery() 
{
	putchar(SYNC);
	putdata(mode, 4);
	putchar(STOP);
}

void gotchar(char b) 
{
	switch (state) {
	case 0:
		if (b==SYNC) state=1;
		ptr=0;
		break;
	case 1:
		buf[ptr++]=b;
		if (ptr == buf[0]+1) {
			state=0;
			csum=0;
			csum_add(buf,buf[0]);
			if (csum==buf[buf[0]]) {
				if (buf[0]==0) {
					serialdiscovery();
					urpc_discovery(); 
				} else
					urpc_handle_incoming((struct urpc_packet*)&buf[1]); 
			}
				
		}
	}
}

static void putdata(const char* data, int len) 
{
	while (len--) {
		putchar(*data);
		csum+=*data++;
	}
}

static void putstr(const char* data) {
	while (*data) {
		putchar(*data);
                csum+=*data++;
	}
	putchar(0x0);
}


void urpc_tx_data(struct urpc_object* obj, char* data, int sz) 
{
	urpc_id_t id;
	urpc_size_t len=0;
	csum=0;
	putchar(SYNC);
	len = (urpc_size_t) sz + sizeof(urpc_size_t);
	if (obj) {
		id = URPC_OBJ_ID(obj);
		len+=sizeof(urpc_id_t);
	}
	putdata(&len,sizeof(urpc_size_t));
	if (obj)
		putdata(&id,sizeof(urpc_id_t));
	putdata(data,sz);
	putchar(csum);
	putchar(STOP);
}

/* [ len | id | flags | name | data | reply | csum ] */
void urpc_tx_object(struct urpc_object* obj) 
{
	urpc_id_t id;
	urpc_size_t len;
	len = strlen(obj->name) + strlen(obj->data) + strlen(obj->reply) + 4;
	len += sizeof(urpc_id_t);
	len += sizeof(urpc_size_t);
	csum=0;
	putchar(SYNC);
	putdata(&len,sizeof(urpc_size_t));
	id = URPC_OBJ_ID(obj);
	putdata(&id,sizeof(urpc_id_t));
	putchar(obj->flags);
	csum+=obj->flags;
	putstr(obj->name);
	putstr(obj->data);
	putstr(obj->reply);
	putchar(csum);
	putchar(STOP);
}
#endif
