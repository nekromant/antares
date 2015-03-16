#ifndef URPC_H
#define URPC_H

#include <arch/antares.h>
#include <stdint.h>

#define URPC_NONE "1"
#define URPC_U8   "2"
#define URPC_U16  "3"
#define URPC_U32  "4"
#define URPC_U64  "5"

#define URPC_I8   "6"
#define URPC_I16  "7"
#define URPC_I32  "8"
#define URPC_I64  "9"

#define URPC_STR    "s"
#define URPC_BIN(n) "b" #n "."


#define urpc_arg_pop(tp, argptr)			\
	(*((tp *) argptr)); argptr += sizeof(tp);	\
	
#define urpc_ret_push(tp, argptr, ret)			\
	(*((tp *) argptr)) = ret; argptr += sizeof(tp);	\
	

#define urpc_pop_u8()  urpc_arg_pop(uint8_t, arg)
#define urpc_pop_u16() urpc_arg_pop(uint16_t, arg)
#define urpc_pop_u32() urpc_arg_pop(uint32_t, arg)
#define urpc_pop_u64() urpc_arg_pop(uint64_t, arg)

#define urpc_pop_i8()  urpc_arg_pop(int8_t, arg)
#define urpc_pop_i16() urpc_arg_pop(int16_t, arg)
#define urpc_pop_i32() urpc_arg_pop(int32_t, arg)
#define urpc_pop_i64() urpc_arg_pop(int64_t, arg)

#define urpc_pop_string()			\
	(char *) arg; arg += strlen(arg) + 1;

#define urpc_push_string(str)			\
	(char *) arg; arg += strlen(arg) + 1;


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
                .arg = arglist URPC_NONE,				\
                .ret = retlist URPC_NONE,				\
		.method = urpc_method_##_name,				\
	};								\
	ANTARES_INIT_LOW(register_method_##_name) {			\
		urpc_register(&urpc_obj_method_##_name);		\
	};								\
	void urpc_method_##_name(void *arg, void *ret)			\
		

#define URPC_EVENT(name, retlist)			\
	struct urpc_object g_urpc_obj_event_##_name = {	\
		.name = _name,				\
                .ret = retlist URPC_NONE,		\
	};						\
	ANTARES_INIT_LOW(register_method_##_name) {     \
		urpc_register(&urpc_obj_event_##_name);	\
	};						\



void urpc_register(struct urpc_object *o);
         
#endif
