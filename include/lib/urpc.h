#ifndef URPC_H
#define URPC_H

#include <arch/antares.h>
#include <stdint.h>

#define URPC_NONE "!"
#define URPC_U8   "1"
#define URPC_U16  "2"
#define URPC_U32  "3"
#define URPC_U64  "4"

#define URPC_I8   "6"
#define URPC_I16  "7"
#define URPC_I32  "8"
#define URPC_I64  "9"

#define URPC_BIN(n) "0" #n "."

#define urpc_arg_get(tp, argptr)			\
	(*((tp *) argptr)); argptr += sizeof(tp);	\
	
#define urpc_ret_put(tp, argptr, ret)			\
	(*((tp *) argptr)) = ret; argptr += sizeof(tp);	\
	

#define urpc_get_u8()  urpc_arg_get(uint8_t, arg)
#define urpc_get_u16() urpc_arg_get(uint16_t, arg)
#define urpc_get_u32() urpc_arg_get(uint32_t, arg)
#define urpc_get_u64() urpc_arg_get(uint64_t, arg)

#define urpc_get_i8()  urpc_arg_get(int8_t, arg)
#define urpc_get_i16() urpc_arg_get(int16_t, arg)
#define urpc_get_i32() urpc_arg_get(int32_t, arg)
#define urpc_get_i64() urpc_arg_get(int64_t, arg)

#define urpc_get_bin(len) \
	(char *) arg; argptr += len;

struct urpc_object { 
	uint8_t id;
	const char* name; 
	const char* arg;
	const char* ret;
	void (*method)(void *arg, void *ret);
};

#define URPC_METHOD(_name, arglist, retlist)				\
	void urpc_method_##_name(void *arg, void *ret);			\
	static struct urpc_object urpc_obj_method_##_name = {		\
		.name = #_name,						\
                .arg = arglist,						\
                .ret = retlist,						\
		.method = urpc_method_##_name,				\
	};								\
	ANTARES_INIT_LOW(register_method_##_name) {			\
		urpc_register(&urpc_obj_method_##_name);		\
	};								\
	void urpc_method_##_name(void *arg, void *ret)			\
		

#define URPC_EVENT(name, retlist)			\
	struct urpc_object g_urpc_obj_event_##_name = {	\
		.name = _name,				\
                .ret = retlist,				\
	};						\
	ANTARES_INIT_LOW(register_method_##_name) {     \
		urpc_register(&urpc_obj_event_##_name);	\
	};						\



void urpc_register(struct urpc_object *o);
         
#endif
