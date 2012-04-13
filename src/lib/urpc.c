
#ifdef CONFIG_URPC_DEBUG
	#define DEBUG
#endif

#include <lib/printk.h>

/* 
 * We have 2 ways to work. 
 * a) Lookup methods in an array (fast, uses memory)
 * b) Do a linear search (slow, uses little memory).
 * With a dozen methods and an attiny2313 b is better. 
 * With ARM and a thousand methods... a is faster. 
 * The choice is yours
 */

/* if func is NULL - it's an event. else - it's a method */
struct urpc_object
{
	const char* name;
	const char* type;
	const void (*func)(void*);
	const int datalen;
	int id; //internally used upon registration, used to send events
#ifdef CONFIG_URPC_LINEAR
	void* next;
#endif
};


static int uprc_counter;

#ifndef CONFIG_URPC_NO_SEATBELTS
static int maxsz;
#endif

static struct urpc_object* head;
#ifdef CONFIG_URPC_LINEAR
static struct urpc_object* prev;
#endif

void urpc_init(void* mem, int msz)
{
		head = mem;
		#ifndef CONFIG_URPC_NO_SEATBELTS
		maxsz=msz;
		#endif
}

void urpc_register_method(struct urpc_object* m)
{
	m->id = urpc_counter++;
#ifdef CONFIG_URPC_LINEAR
	if (prev)
		prev->next = m;
	else
		head = m;
	prev=m;
#endif
	
#ifndef CONFIG_URPC_NO_SEATBELTS
		if (m->id < msz)
		{
#endif
	#ifdef CONFIG_URPC_ARRAY
	head[m->id]=m;
	#endif
#ifndef CONFIG_URPC_NO_SEATBELTS
		} else
		{
			dbg("urpc: WARN! Out of space in the array, object dropped\n");
		}
#endif
	dbg("urpc: registered %s.%s\n" m->type, m->name);
	
}

void urpc_discover()
{
	
}

void urpc_call(int id, void* data)
{
	
}

void urpc_trigger(struct urpc_object* o)
{
	
}