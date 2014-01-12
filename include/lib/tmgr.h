#ifndef INCLUDE_LIB_TMGR_H
#define INCLUDE_LIB_TMGR_H

#include <arch/antares.h>
#include <generic/macros.h>
#include <stdint.h>

/* TODO: remove it ASAP */
#ifndef ANTARES_ENABLE_IRQS
#define ANTARES_ENABLE_IRQS()
#define ANTARES_DISABLE_IRQS()
#endif

#if defined(CONFIG_LIB_TMGR_TIME_32)
typedef int32_t tmgr_uptime_t;
#elif defined(CONFIG_LIB_TMGR_TIME_64)
typedef int64_t tmgr_uptime_t;
#else
#error "tmgr: Wrong tmgr_uptime_t size (must be 32 or 64 bits)"
#endif

#define tmgr_ticks_to_us(ticks) (1000000U * (ticks) / CONFIG_LIB_TMGR_FQ)
#define tmgr_ticks_to_ms(ticks) (1000 * (ticks) / CONFIG_LIB_TMGR_FQ)
#define tmgr_ticks_to_s(ticks) ((ticks) / CONFIG_LIB_TMGR_FQ)
#define tmgr_us_to_ticks(us) ((us) * CONFIG_LIB_TMGR_FQ / 1000000U)
#define tmgr_ms_to_ticks(ms) ((ms) * CONFIG_LIB_TMGR_FQ / 1000)
#define tmgr_s_to_ticks(s) ((s) * CONFIG_LIB_TMGR_FQ)

#define tmgr_get_fq() CONFIG_LIB_TMGR_FQ

/* Timer initializer macros.
 * Creates right structure for timer and fill it
 */
#define TMGR_TIMER_INIT(_function, _data, _expires) { .expires = (_expires), .func = (_function), .data = (_data) }
#define TMGR_DEFINE_TIMER(_name, _function, _data, _expires) tmgr_timer_t _name = TMGR_TIMER_INIT(_function, _data, _expires)
#define TMGR_SETUP_TIMER(_timer, _function, _data, _expires) _timer.func = (_function); _timer.data = (_data); _timer.expires = (_expires)

#define TMGR_DELAYED(delay) (tmgr_get_uptime() + delay)

/* Timers are making doubly-linked list, it's good for
 * fast search and removal of timer object.
 */
typedef struct struct_tmgr_timer_t {
        tmgr_uptime_t expires;
        void (*func)(uint8_t *);
        uint8_t *data;

        struct struct_tmgr_timer_t *next;
        struct struct_tmgr_timer_t *prev;
} tmgr_timer_t;

void tmgr_process(void);
void tmgr_interrupt(void);

void tmgr_add_timer(tmgr_timer_t *timer);
void tmgr_mod_timer(tmgr_timer_t *timer, tmgr_uptime_t expires);
void tmgr_del_timer(tmgr_timer_t *timer);

tmgr_uptime_t tmgr_get_uptime(void);
void tmgr_delay(tmgr_uptime_t time);

#endif
