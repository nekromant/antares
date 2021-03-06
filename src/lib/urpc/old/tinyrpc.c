#include <arch/antares.h>
#include <lib/urpc.h>
#include <string.h>

#define COMPONENT "urpc-core"
#define DEBUG_LEVEL 4
#include <lib/printk.h>
#include <lib/panic.h>
#include <lib/circ_buf.h>

urpc_id_t urpc_register_object(struct urpc *u, struct urpc_object *o)
{
	if (u->num_objects >= u->max_objects) {
		panic("Available data storage exhausted. increase urpc max_objects!");
	}
	u->objects[u->num_objects] = o;
	urpc_id_t id = u->num_objects++;
	dbg("Registering object: %s\n", o->name);
	return id;
}

void urpc_call(struct urpc *u, urpc_id_t id, void *data, urpc_size_t sz)
{
	if (id >= u->num_objects) {
		warn("Caller id out of bounds\n");
		return; /* bogus call to nowhere */
	}
	/* 
	 * Events don't have method field set, 
	 * and shouldn't be called, ignore them 
	 */
	if (u->objects[id]->method)
		u->objects[id]->method(id, data, sz);
}


void urpc_evt_write(struct urpc *u, char *data, urpc_size_t sz)
{
	int tocopy; 
	do { 
		tocopy = min_t(int, sz, 
			       CIRC_SPACE_TO_END(
				       u->evt_head, 
				       u->evt_tail, 
				       CONFIG_URPC_EBLEN));
		memcpy(&u->evt_buf[u->evt_head], data, tocopy);
		sz-=tocopy;
		u->evt_head = (u->evt_head + tocopy) & (CONFIG_URPC_EBLEN - 1);
	} while (sz);
}

void urpc_respond(struct urpc *u, urpc_id_t id, char *data, urpc_size_t sz)
{
	if (id >= u->num_objects) {
		warn("Caller id out of bounds\n");
		return; /* bogus call to nowhere */
	}	
	
	ANTARES_ATOMIC_BLOCK() {
		u->handle_event(id, data, sz);
	}
}
