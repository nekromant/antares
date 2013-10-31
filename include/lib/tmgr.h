#ifndef INCLUDE_LIB_TMGR_H
#define INCLUDE_LIB_TMGR_H

#include "inttypes.h"
#include <arch/antares.h>
#include <generic/macros.h>

#if defined(CONFIG_LIB_TMGR_TIME_16)
typedef int16_t tmgr_time_t;
#elif defined(CONFIG_LIB_TMGR_TIME_32)
typedef int32_t tmgr_time_t;
#elif defined(CONFIG_LIB_TMGR_TIME_64)
typedef int64_t tmgr_time_t;
#else
#error "tmgr: Wrong tmgr_time_t size (must be 16, 32 or 64 bits)"
#endif

#define tmgr_ticks_to_us(ticks) (1000000U * (ticks) / tmgr_get_fq())
#define tmgr_ticks_to_ms(ticks) (1000 * (ticks) / tmgr_get_fq())
#define tmgr_ticks_to_s(ticks) ((ticks) / tmgr_get_fq())
#define tmgr_us_to_ticks(us) ((us) * tmgr_get_fq() / 1000000U)
#define tmgr_ms_to_ticks(ms) ((ms) * tmgr_get_fq() / 1000)
#define tmgr_s_to_ticks(s) ((s) * tmgr_get_fq())

typedef struct struct_tmgr_task_t {
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

#endif
