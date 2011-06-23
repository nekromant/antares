// #ifndef putc_func
// #error "Please define your putc_func for debug library"
// #endif
// 
// #ifndef puts_func
// #error "We do need a puts_func for debug to work"
// #endif
#include "uart.h"
#define putc_func comm_putc
#define puts_func comm_puts

#define DBG_MAGIC	0xf1
#define INF_MAGIC	0xf2
#define WRN_MAGIC	0xf3
#define ERR_MAGIC	0xf4

#define DMP8 	0xf6
#define DMP16 	0xf7
#define DMP32 	0xf8


#ifdef _DBG_ENABLED
#define DBG(text) putc_func(DBG_MAGIC); puts_func(text); 
#else
#define DBG(text) ;;//text
#endif

#ifdef _INF_ENABLED
#define INF(text) putc_func(DBG_MAGIC); puts_func(text);
#else
#define INF(text) ;;
#endif

#ifdef _WRN_ENABLED
#define WRN(text) putc_func(DBG_MAGIC); puts_func(text);
#else
#define WRN(text) ;;
#endif

#ifdef _ERR_ENABLED
#define ERR(text) putc_func(DBG_MAGIC); puts_func(text); putc_func('\n');
#else
#define ERR(text) ;;
#endif


