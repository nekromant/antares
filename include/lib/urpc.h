#ifndef __URPC_H
#define __URPC_H

/* Packet is method call */ 
#define FLAG_URPC_METHOD       1
/* Packet is event data */
#define FLAG_URPC_EVENT        2

#if CONFIG_URPC_SZB == 1
typedef unsigned char urpc_size_t;
#elif CONFIG_URPC_SZB == 2
typedef uint16_t urpc_size_t;
#elif CONFIG_URPC_SZB == 4
typedef uint32_t urpc_size_t;
#endif

#if CONFIG_URPC_IDB == 1
typedef unsigned char urpc_id_t;
#elif CONFIG_URPC_IDB == 2
typedef uint16_t urpc_id_t;
#elif CONFIG_URPC_IDB == 4
typedef uint32_t urpc_id_t;
#endif


/* 'data' actually serves to be the pointer to the first byte 
 * But some compilers don't like flexible arrays (sdcc) 
 */

struct urpc_packet {
	urpc_id_t id;
	unsigned char data;
};


struct urpc_object {
	const char flags;
	const char* name;
	const char* data;
	const char* reply;
	void (*method)(char* data); 
};

#define URPC_OBJ_ID(obj) \
	(((char*)obj - (char*)&urpc_exports[0])/sizeof(struct urpc_object))

/* Transport layer */
void urpc_tx_data(struct urpc_object* obj, char* data, int sz);
void urpc_tx_object(struct urpc_object* obj);
/* Callback */
void urpc_handle_incoming(struct urpc_packet* pck);

void urpc_discovery();
void urpc_loop(); /* Processing loop */
void urpc_respond(char* data, int sz);

extern struct urpc_object urpc_exports[];

#endif
