#ifndef INCLUDE_LIB_TMGR_H
#define INCLUDE_LIB_TMGR_H

#include "inttypes.h"

#ifndef NULL
    #define NULL 0
#endif

#if defined(CONFIG_LIB_TMGR_TIME_16)
typedef int16_t tmgr_time_t;
#elif defined(CONFIG_LIB_TMGR_TIME_32)
typedef int32_t tmgr_time_t;
#elif defined(CONFIG_LIB_TMGR_TIME_64)
typedef int64_t tmgr_time_t;
#else
    // TODO: format errror message
    #error "Wrong tmgr_time_t size (must be 16, 32 or 64 bits)"
#endif

typedef struct struct_tmgr_task_t{
    tmgr_time_t time;
    void (*func)(void);
    struct struct_tmgr_task_t * next;
} tmgr_task_t;

void tmgr_tick(void);
void tmgr_timer(void);
void tmgr_register(tmgr_task_t * task, void (*function)(void), tmgr_time_t delay);

#ifdef CONFIG_LIB_TMGR_UPTIME
tmgr_time_t tmgr_get_uptime(void);
#endif
void tmgr_delay(tmgr_time_t time);

void tmgr_set_fq(tmgr_time_t fq);
tmgr_time_t tmgr_get_fq(void);

#ifdef CONFIG_LIB_TMGR_EXTRA
tmgr_time_t tmgr_ticks_to_us(tmgr_time_t ticks);
tmgr_time_t tmgr_ticks_to_ms(tmgr_time_t ticks);
tmgr_time_t tmgr_ticks_to_s(tmgr_time_t ticks);
tmgr_time_t tmgr_us_to_ticks(tmgr_time_t us);
tmgr_time_t tmgr_ms_to_ticks(tmgr_time_t ms);
tmgr_time_t tmgr_s_to_ticks(tmgr_time_t s);
#endif

#endif
