#include <arch/antares.h>
#include <lib/urpc.h>
#include <lib/panic.h>
#include <lib/printk.h>

static struct urpc_object *registry[CONFIG_LIB_URPC_OCOUNT];
static uint8_t lastid;

URPC_METHOD(hello, 
	    URPC_U32 "arg1" URPC_U32 "arg2", 
	    URPC_NONE
	)
{
	
}

int urpc_raise_event(struct urpc_object *o, ...)
{
	
}

void urpc_call(uint8_t id, void *arg, void *ret)
{
	
}

void urpc_register(struct urpc_object *o)
{ 
	if (lastid >= CONFIG_LIB_URPC_OCOUNT) { 
		panic("Too many objects");
	}
	registry[lastid] = o;
	o->id = (uint8_t) lastid++;
}



