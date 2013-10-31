#include <lib/tmgr.h>

static volatile tmgr_time_t sys_uptime = 0;
static volatile int32_t sys_fq = 1;

static tmgr_task_t *fast_queue = NULL;
static tmgr_task_t *main_queue = NULL;

#ifdef CONFIG_LIB_TMGR_COUNTDOWN
static tmgr_time_t delay;
#endif

/**
 * @section Task Manager core
 * @{
 */

void tmgr_timer(void)
{
#if defined(CONFIG_LIB_TMGR_UPTIME)
        sys_uptime++;
#elif defined(CONFIG_LIB_TMGR_COUNTDOWN)
        /* Implement delay feature */
        if (delay > 0)
                delay--;

        tmgr_task_t * queue = main_queue;
        while (queue != NULL) {
                queue->time --;
                queue = queue->next;
        }
#endif
}

void tmgr_register(tmgr_task_t * task, void (*function)(void), tmgr_time_t delay)
{
#if defined(CONFIG_LIB_TMGR_UPTIME)
        task->time = sys_uptime + delay;
#elif defined(CONFIG_LIB_TMGR_COUNTDOWN)
        task->time = delay;
#endif
        task->func = function;
        
        /* these lines are atomic */
        ANTARES_DISABLE_IRQS();
        task->next = fast_queue;
        fast_queue = task;
        ANTARES_ENABLE_IRQS();
}

void tmgr_tick(void)
{
        /* Check fast queue */
        if (fast_queue != NULL) {
                /* Attach fast queue to main queue */
                ANTARES_DISABLE_IRQS();
                tmgr_task_t *fast = fast_queue,
                           *current = main_queue,
                           **last = &main_queue;

                fast_queue = NULL;
                ANTARES_ENABLE_IRQS();

                while(fast != NULL)
                {
                        #if defined(CONFIG_LIB_TMGR_UPTIME)
                        while(current != NULL && current->time < fast->time)
                        #elif defined(CONFIG_LIB_TMGR_COUNTDOWN)
                        while(current != NULL && current->time > fast->time)
                        #endif
                        {
                                current = current->next;
                                last = &((*last)->next);
                        }

                        fast->next = current;
                        *last = fast;
                        
                        fast = fast->next;
                }
        }

        /* Run main queue tasks */
        if (main_queue != NULL) {
                #if defined(CONFIG_LIB_TMGR_UPTIME)
                while (main_queue != NULL && main_queue->time <= sys_uptime) {
                #elif defined(CONFIG_LIB_TMGR_COUNTDOWN)
                while (main_queue != NULL && main_queue->time <= 0) {
                #endif
                        main_queue->func();
                        main_queue = main_queue->next;
                }
        }
}

/**
 * @}
 *
 * @section Task Manager Uptime functions
 * @{
 */

tmgr_time_t tmgr_get_uptime(void)
{
        return sys_uptime;
}

void tmgr_delay(tmgr_time_t time)
{
#if defined(CONFIG_LIB_TMGR_UPTIME)
        time += sys_uptime;
        while (time > sys_uptime);
#elif defined(CONFIG_LIB_TMGR_COUNTDOWN)
        delay = time;
        while (delay > 0);
#endif
}

/**
 * @}
 */

/**
 * @section Time conversion functions
 * @{
 */
void tmgr_set_fq(tmgr_time_t fq)
{
        sys_fq = fq;
}

tmgr_time_t tmgr_get_fq(void)
{
        return sys_fq;
}

/**
 * @}
 */
