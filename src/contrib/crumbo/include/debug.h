// #ifndef putc_func
// #error "Please define your putc_func for debug library"
// #endif
// 
// #ifndef puts_func
// #error "We do need a puts_func for debug to work"
// #endif
#include "uart.h"
#define putc_func comm_putc
#define puts_func(text) comm_sync();comm_send_pgms(PSTR(text)); 

#define DBG_MAGIC	0xf1
#define INF_MAGIC	0xf2
#define WRN_MAGIC	0xf3
#define ERR_MAGIC	0xf4

#define DMP8 	0xf6
#define DMP16 	0xf7
#define DMP32 	0xf8


#ifdef _DBG_ENABLED
#ifndef CONFIG_CONTRIB_CRUMBO_DEBUG_RP
  #define DBG(text) putc_func(DBG_MAGIC); puts_func(text); 
  #else
  #define DBG(text) puts_func("DBG: " text "\r\n"); 
#endif
#else
#define DBG(text) ;;//text
#endif

#ifdef _INF_ENABLED
#ifndef CONFIG_CONTRIB_CRUMBO_DEBUG_RP
  #define INF(text) putc_func(INF_MAGIC); puts_func(text); 
  #else
  #define INF(text) puts_func("INF: " text "\r\n"); 
#endif
#else
#define INF(text) ;;//text
#endif

#ifdef _WRN_ENABLED
#ifndef CONFIG_CONTRIB_CRUMBO_DEBUG_RP
  #define WRN(text) putc_func(WRN_MAGIC); puts_func(text); 
  #else
  #define WRN(text) puts_func("WRN: " text "\r\n"); 
#endif
#else
#define WRN(text) ;;//text
#endif

#ifdef _ERR_ENABLED
#ifndef CONFIG_CONTRIB_CRUMBO_DEBUG_RP
  #define ERR(text) putc_func(ERR_MAGIC); puts_func(text); 
  #else
  #define ERR(text) puts_func("ERR: " text "\r\n"); 
#endif
#else
#define ERR(text) ;;//text
#endif

typedef union {
  uint32_t v;
  uint8_t bytes[4];
} _u32;


void dump8(uint8_t data);
void dump16(uint16_t data);
void dump32(_u32 data);

