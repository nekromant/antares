#ifndef URPC_H
#define URPC_H

#include <arch/antares.h>
#include <stdint.h>

#define R_URPC_NONE 0x00
#define R_URPC_U8   0x01
#define R_URPC_U16  0x02
#define R_URPC_U32  0x03
#define R_URPC_U64  0x04

#define R_URPC_I8   0x05
#define R_URPC_I16  0x06
#define R_URPC_I32  0x07
#define R_URPC_I64  0x08

#define R_URPC_BIN  0x09

#define URPC_NONE "\x00\x00"
#define URPC_U8   "\x00\x01"
#define URPC_U16  "\x00\x02"
#define URPC_U32  "\x00\x03"
#define URPC_U64  "\x00\x04"

#define URPC_I8   "\x00\x05"
#define URPC_I16  "\x00\x06"
#define URPC_I32  "\x00\x07"
#define URPC_I64  "\x00\x08"

#define URPC_BIN(n) "\x00\x09" n


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
