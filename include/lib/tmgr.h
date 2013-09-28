#ifndef INCLUDE_LIB_TMGR_H
#define INCLUDE_LIB_TMGR_H

#ifndef NULL
    #define NULL 0
#endif

/**
 * TODO: insert platform-dependent includes
 */

typedef int32_t uptime_t;
typedef int32_t delay_t;

typedef struct {
    uptime_t time;
    void *func(void);
    task_t * next;
} task_t;

void tmgr_tick(void);
void tmgr_timer(void);
void tmgr_register(task_t * task);

uptime_t tmgr_get_uptime(void);
void tmgr_delay(uptime_t time);

#endif
