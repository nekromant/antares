#include <lib/urpc.h>
#include <lib/printk.h>
#include <vaarg.h>

static int urpc_counter;
static struct urpc_object* head;
static struct urpc_object* prev;
static volatile int urpc_lock;

void urpc_register(struct urpc_object* m)
{
	m->id = urpc_counter++;
	if (prev)
		prev->next = m;
	else
		head = m;
	prev = m;
	dbg("urpc: registered %s %s.%s #id %d \r\n", m->func ? "method" : "event", m->type, m->name, m->id);
}

void urpc_list_methods()
{
	struct urpc_object* m = head;
	printk("---cut---\n\r");
	do {
		printk("urpc: %s %s.%s; #id is %d \r\n", m->func ? "method" : "event", m->type, m->name, m->id);
		m=m->next;
	} while(m);
	printk("---cut---\n\r");
	
}


/* generate lookup table */
static struct urpc_object **lut;

void urpc_generate_lut()
{
	struct urpc_object* m = head;
	int i=0;
	dbg("urpc: generating lookup cache\n\r");
	/* get rid of old lut, if any */
	if (lut) free(lut);
	lut = malloc(sizeof(void*) * urpc_counter);
	if (!lut) {
		dbg("urpc: out of memory while generating look-up table");
		while(1);;
	}
	do {
		lut[i] = m;
		i++;
		m=m->next;
	} while(m);
	dbg("urpc: done generating lookup cache for %d objects\n\r", i);
}

/* enables event handling and processing */
void urpc_enable()
{
	urpc_lock = URPC_ENABLED;
}
/* disables event handling and processing */
void urpc_disable()
{
	urpc_lock = URPC_DISABLED;
}

void urpc_seatbelt(char* format, ...)
{
	

}

void* urpc_call(int id, struct urpc_object* m)
{
	
}


void * urpc_call_by_id(int id, void* data)
{
if (URPC_DISABLED == urpc_lock)
	{
		dbg("urpc: skipping call due to disabled state");
		return NULL;
	}

#ifndef CONFIG_URPC_NO_SEATBELTS
		if (id >= urpc_counter)
		{
			dbg("urpc: ---- OOOPS ----\n\r");
			dbg("urpc: protocol error, refusing to crash system.\n\r");
			dbg("urpc: %d >= %d\n\r");		
			return NULL;
		}
#endif

#ifdef CONFIG_URPC_LUT
		 
		
#else
	       
struct urpc_object* m = head;
		int i;
		while(i <= id)
		do {
			
			m=m->next;
		}
#endif		

}



void urpc_event(struct urpc_object* m, void* data)
{

}
