#ifndef URPC_H
#define URPC_H

#include <arch/antares.h>
#include <stdint.h>

#if CONFIG_URPC_SZB == 1
typedef uint8_t urpc_size_t;
#define STAG 0x1
#elif CONFIG_URPC_SZB == 2
typedef uint16_t urpc_size_t;
#define STAG 0x2
#elif CONFIG_URPC_SZB == 4
typedef uint32_t urpc_size_t;
#define STAG 0x4
#else
#error "Something nasty just happened"
#endif

#if CONFIG_URPC_IDB == 1
typedef uint8_t urpc_id_t;
#define ITAG 0x1
#elif CONFIG_URPC_IDB == 2
typedef uint16_t urpc_id_t;
#define ITAG 0x2
#elif CONFIG_URPC_IDB == 4
#define ITAG 0x4
typedef uint32_t urpc_id_t;
#else
#error "Something nasty just happened"
#endif


#if defined(CONFIG_ARCH_LE)
#define ENDIANNESS 'l'
#elif defined(CONFIG_ARCH_BE)
#define ENDIANNESS 'b'
#else  
#define ENDIANNESS '?'
#endif

struct urpc_object {
	const char* name;
	const unsigned char* argfmt;
	const unsigned char* responsefmt;
	void (*method)(urpc_id_t id, void *arguments, urpc_size_t arglen); 
};

#define CONFIG_URPC_EBLEN  128
struct urpc {
	void (*notify)(); /* Called when new event added to queue, optional */
	urpc_id_t num_objects;
	urpc_id_t max_objects;
	struct urpc_object **objects; 
	int  evt_tail;
	int  evt_head; 
	char evt_buf[CONFIG_URPC_EBLEN];
};


/** 
 * Registers an urpc object (event or method) with urpc instance.
 * Do not call this directly! Use URPC_ADD_METHOD/URPC_ADD_EVENT
 * macros.
 *
 * @param u urpc instace
 * @param o urpc object
 * 
 * @return newly registered object id
 */
urpc_id_t urpc_register_object(struct urpc *u, struct urpc_object *o);

/** 
 * Call an urpc object. 
 * This function should be invoked by the transport layer
 * 
 * @param u urpc instance
 * @param id object id
 * @param data arguments
 */
void urpc_call(struct urpc *u, urpc_id_t id, void *data, urpc_size_t datalen);

/* Respond with data for a method call, or raise the event */

void urpc_respond(struct urpc *u, urpc_id_t id, char *data, urpc_size_t datalen);



#define URPC_REGISTER(__u, __obj)			\
	ANTARES_INIT_LOW(urpc_add_ ## __obj) {	\
		urpc_register_object(__u, &__obj);	\
	}

#define URPC_REGISTER_EVENT(__u, __obj, __id)				\
	ANTARES_INIT_LOW(urpc_add_ ## __obj ) {				\
		__id = urpc_register_object(__u, &__obj);			\
	}


#define URPC_DECLARE_INSTANCE(name, evt_handler, max_obj)		\
	static struct urpc_object * name ## _data[max_obj];		\
	static struct urpc name =  {					\
		.action = evt_handler,					\
		.num_objects = 0,					\
		.max_objects = max_obj,					\
		.objects = (struct urpc_object *) name ## _data		\
	};




#define U_UINT8   "1"
#define U_UINT16  "2"
#define U_UINT32  "4"
#define U_UINT64  "8"

#define U_SINT8   "a"
#define U_SINT16  "b"
#define U_SINT32  "c"
#define U_SINT64  "d"

#define U_STR     "s"

#define U_SBIN(len) "B\x" #len

                                                                                   
#endif
