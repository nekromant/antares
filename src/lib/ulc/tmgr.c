#include <lib/tmgr.h>

static volatile uptime_t sys_uptime = 0;
static volatile int32_t sys_fq = 0;

static task_t *fast_queue = NULL;
static task_t *main_queue = NULL;

/**
 * @section Task Manager core
 * @{
 */

void tmgr_timer(void)
{
    sys_uptime++;
}

// TODO: make it atomic
void tmgr_register(task_t * task)
{
    task->next = fast_queue;
    fast_queue = task;
}

void tmgr_tick(void)
{
    // 1. Check fast queue
    if(fast_queue != NULL)
    {
        // Attach fast queue to main queue
        task_t *fast = fast_queue,
               *current = main_queue,
               **last = &main_queue;

        fast_queue = NULL;

        while(fast != NULL)
        {
            while(current != NULL && current->time < fast->time)
            {
                current = current->next;
                last = &((*last)->next);
            }

            fast->next = current;
            *last = fast;
            
            fast = fast->next;
        }
    }

    // 2. Run main queue
    if(main_queue != NULL)
    {
        while(main_queue != NULL && main_queue->time <= sys_uptime)
        {
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

uptime_t tmgr_get_uptime(void)
{
    return sys_uptime;
}

void tmgr_delay_ticks(uptime_t time)
{
    time += sys_uptime;

    while(time > sys_uptime);
}

/**
 * @}
 */

/**
 * @section Time conversion functions
 * @{
 */
void tmgr_set_fq(int32_t fq)
{
    sys_fq = fq;
}

int32_t tmgr_get_fq(void)
{
    return sys_fq;
}

int32_t tmgr_ticks_to_us(int32_t ticks)
{
    return 1000000U * ticks / sys_fq;
}

int32_t tmgr_ticks_to_ms(int32_t ticks)
{
    return 1000 * ticks / sys_fq;
}

int32_t tmgr_ticks_to_s(int32_t ticks)
{
    return ticks / sys_fq;
}

int32_t tmgr_us_to_ticks(int32_t us)
{
    return us * sys_fq / 1000000U;
}

int32_t tmgr_ms_to_ticks(int32_t ms)
{
    return ms * sys_fq / 1000;
}

int32_t tmgr_s_to_ticks(int32_t s)
{
    return s * sys_fq;
}
/**
 * @}
 */
