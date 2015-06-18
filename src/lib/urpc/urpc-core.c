#define COMPONENT "urpc-core"
#define DEBUG_LEVEL 88

#include <arch/antares.h>
#include <lib/urpc.h>
#include <lib/panic.h>
#include <lib/printk.h>
#include <stdarg.h>
#include <alloca.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>



static struct urpc_object *registry[CONFIG_LIB_URPC_OCOUNT];
static uint8_t lastid;

int urpc_get_registry(struct urpc_object ***reg)
{
	*reg = registry;
	return lastid;
}

/* Some data types may be promoted that screws things up */
#define va_arg_ex(ret, ap, type, ptype)			\
			ret = (type) va_arg(ap, ptype);	\

void urpc_raise_event(struct urpc_object *o, ...)
{
	va_list ap;
	const unsigned char *rfmt = (unsigned char *) o->ret;
	va_start(ap, o);
	int tocopy;
	void *from;
	union { 
		uint8_t  u8;
		uint16_t u16;
		uint32_t u32;
		uint64_t u64;
		int8_t  i8;
		int16_t i16;
		int32_t i32;
		int64_t i64;

	} tmp;

	urpc_backend_evt_start(o);
	while (true) {
		switch (*rfmt) {
		case URPC_U8_TOK:
			va_arg_ex(tmp.u8, ap, uint8_t, unsigned int);
			from = &tmp.u8;
			tocopy = sizeof(uint8_t);;
			break;
		case URPC_S8_TOK: {
			va_arg_ex(tmp.i8, ap, int8_t, int);
			from = &tmp.i8;
			tocopy = sizeof(int8_t);;
			break;
		}
		case URPC_U16_TOK:
			va_arg_ex(tmp.u16, ap, uint16_t, unsigned int);
			from = &tmp.u16;
			tocopy = sizeof(uint16_t);;
			break;
		case URPC_S16_TOK:
			va_arg_ex(tmp.i16, ap, int16_t, int);
			from = &tmp.i16;
			tocopy = sizeof(int16_t);;
			break;
		case URPC_U32_TOK:
		case URPC_S32_TOK:
			tmp.u32 = va_arg(ap, uint32_t);
			from = &tmp.u32;
			tocopy = sizeof(uint32_t);;
			break;
		case URPC_U64_TOK:
		case URPC_S64_TOK:
			tmp.u64 = va_arg(ap, uint64_t);
			from = &tmp.u64;
			tocopy = sizeof(uint64_t);;
			break;
		case URPC_BIN_TOK:
			from = va_arg(ap, char *);
			tocopy = atoi((char *) rfmt);
			while (*rfmt && (*(++rfmt) != '.'));
			break;
		case 0x0:
			urpc_backend_evt_finish();	
			return;
		default: 
			panic("Invalid token");
			return;
		}
		urpc_backend_evt_write(from, tocopy);	
		rfmt++;
	}
}

void urpc_call(uint8_t id, void *arg, void *ret)
{
	struct urpc_object *o;
	int len; 
	char *rstart = ret; 
	if (id >= CONFIG_LIB_URPC_OCOUNT)
		return;
	o = registry[id];
	if (o->method)
		o->method(arg, &ret);
	len = ((char *) ret) - rstart;

	/* Issue completion event */ 
	urpc_backend_evt_start(o);
	urpc_backend_evt_write(ret, len);
	urpc_backend_evt_finish();	
}

void urpc_register(struct urpc_object *o)
{ 
	if (lastid >= CONFIG_LIB_URPC_OCOUNT) { 
		panic("Too many objects");
	}
	dbg("Registering object: %s\n", o->name);
	dbg("Arg len %d\n",  urpc_calc_buffer_size((unsigned char *) o->arg));
	dbg("Ret len %d\n",  urpc_calc_buffer_size((unsigned char *) o->ret));
	registry[lastid] = o;
	o->id = (uint8_t) lastid++;
}
