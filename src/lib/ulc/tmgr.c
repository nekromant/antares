#include <lib/tmgr.h>

static uptime_t sys_uptime = 0;

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
        task_t * fast = fast_queue,
               * current = main_queue,
               ** last = &main_queue;

        fast_queue = NULL;

        while(fast->next != NULL)
        {
            while(current != NULL && current->time < fast->time)
            {
                current = current->next;
                last = &(last->next);
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

void tmgr_delay(uptime_t time)
{
    time += sys_uptime;

    while(time > sys_uptime);
}
