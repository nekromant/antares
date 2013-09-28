#ifndef INCLUDE_LIB_TMGR_H
#define INCLUDE_LIB_TMGR_H

#ifdef CONFIG_ARCH_NATIVE
    #include <stdlib.h>
    #include <inttypes.h>
#endif

#ifndef NULL
    #define NULL 0
#endif

/**
 * TODO: insert platform-dependent includes
 */

/**
 * TODO: uptime_t should be platform-dependent (or application-dependent)
 */
typedef int32_t uptime_t;

typedef struct struct_task_t{
    uptime_t time;
    void (*func)(void);
    struct struct_task_t * next;
} task_t;

void tmgr_tick(void);
void tmgr_timer(void);
void tmgr_register(task_t * task);

uptime_t tmgr_get_uptime(void);
void tmgr_delay(uptime_t time);

void tmgr_set_fq(int32_t fq);
int32_t tmgr_get_fq(void);

#endif
