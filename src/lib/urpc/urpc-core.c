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



static struct urpc_object *registry[CONFIG_LIB_URPC_OCOUNT];
static uint8_t lastid;


static inline const unsigned char* next_arg(const unsigned char *args)
{
	printk("current %x\n", *args);
	if (*args == R_URPC_NONE)
		return NULL;
	while (*args++ != 0x0);;
	printk("next %x\n", *args);
	return args;
}


inline int urpc_type_size(const unsigned char *fmt) {
	int increment;
	switch (*fmt) 
	{
	case 0:
		increment = 0;
	case R_URPC_U8:
	case R_URPC_I8:
		increment = sizeof(uint8_t);;
		break;
	case R_URPC_U16:
	case R_URPC_I16:
		increment = sizeof(uint16_t);;
		break;
	case R_URPC_U32:
	case R_URPC_I32:
		increment = sizeof(uint32_t);;
		break;
	case R_URPC_U64:
	case R_URPC_I64:
		increment = sizeof(uint64_t);;
		break;
	case R_URPC_BIN:
		fmt++;
		increment = (size_t) *fmt;
		break;
	default:
		increment = -1;
	}	
	return increment;
};


size_t urpc_calc_buffer_size(const unsigned char *fmt)
{
	int increment;
	size_t ret = 0;
	fmt++; /* Skip first 0x0 */
	while (*fmt) {
		increment = urpc_type_size(fmt);
		if (increment < 0) { 
			panic("urpc: Invalid format token!");
		}
		ret+=increment;
		fmt = next_arg(fmt);
	}
	return ret;
}


static int  get_copy_size(const unsigned char *rfmt)
{
	
}

/* Some data types may be promoted that screws things up */
#define va_arg_ex(ret, ap, type, ptype)			\
			ret = (type) va_arg(ap, ptype);	\

bool urpc_raise_event(struct urpc_object *o, ...)
{
	va_list ap;
	const unsigned char *rfmt = (unsigned char *) o->ret;
	size_t evtbufsize = urpc_calc_buffer_size(rfmt);
	char *buf = alloca(evtbufsize);
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

	while (*rfmt) {
		
		switch (*rfmt) {
		case R_URPC_U8:
			va_arg_ex(tmp.u8, ap, uint8_t, unsigned int);
			from = &tmp.u8;
			tocopy = sizeof(uint8_t);;
			break;
		case R_URPC_I8: {
			va_arg_ex(tmp.i8, ap, int8_t, int);
			from = &tmp.i8;
			tocopy = sizeof(int8_t);;
			break;
		}
		case R_URPC_U16:
			va_arg_ex(tmp.u16, ap, uint16_t, unsigned int);
			from = &tmp.u16;
			tocopy = sizeof(uint16_t);;
			break;
		case R_URPC_I16:
			va_arg_ex(tmp.i16, ap, int16_t, int);
			from = &tmp.i16;
			tocopy = sizeof(int16_t);;
			break;
		case R_URPC_U32:
		case R_URPC_I32:
			tmp.u32 = va_arg(ap, uint32_t);
			from = &tmp.u32;
			tocopy = sizeof(uint32_t);;
			break;
		case R_URPC_U64:
		case R_URPC_I64:
			tmp.u64 = va_arg(ap, uint64_t);
			from = &tmp.u64;
			tocopy = sizeof(uint64_t);;
			break;
		case R_URPC_BIN:
			rfmt++;
			from = va_arg(ap, char *);
			tocopy = (int) *rfmt;	
			break;
		default: 
			panic("Invalid token");
			return false;
		}
		memcpy(buf, from, tocopy);
		buf += tocopy;
		rfmt = next_arg(rfmt);
	}
	return urpc_backend_evt_deliver(o, buf, evtbufsize);
}

void urpc_call(uint8_t id, void *arg, void *ret)
{
	if (id >= CONFIG_LIB_URPC_OCOUNT)
		return;
	if (registry[id]->method)
		registry[id]->method(arg, ret);
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
