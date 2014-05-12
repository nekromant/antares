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

void urpc_evt_read(struct urpc *u, char *data, urpc_size_t sz)
{
	int tocopy; 
	do { 
		tocopy = min_t(int, sz, 
			       CIRC_CNT_TO_END(
				       u->evt_head, 
				       u->evt_tail, 
				       CONFIG_URPC_EBLEN));
		memcpy(data, &u->evt_buf[u->evt_tail], tocopy);
		sz-=tocopy;
		u->evt_tail = (u->evt_tail + tocopy) & (CONFIG_URPC_EBLEN - 1);
	} while (sz); 	
}


void urpc_respond(struct urpc *u, urpc_id_t id, char *data, urpc_size_t sz)
{
	if (id >= u->num_objects) {
		warn("Caller id out of bounds\n");
		return; /* bogus call to nowhere */
	}	
	
	ANTARES_ATOMIC_BLOCK() {
		if ((sz + sizeof(urpc_id_t) + sizeof(urpc_size_t)) > 
		    CIRC_SPACE(u->evt_head, u->evt_tail, CONFIG_URPC_EBLEN)) { 
			warn("Not enough room in event ring, dropping event\n");
			return;
		}

		urpc_evt_write(u, (char *) &id, sizeof(urpc_id_t));
		urpc_evt_write(u, (char *) &sz, sizeof(urpc_size_t));
		urpc_evt_write(u, data, sz);

		if (u->notify) { 
			u->notify();
		};
	}
}
