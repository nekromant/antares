#include <lib/tmgr.h>

static volatile tmgr_uptime_t sys_uptime = 0;
static volatile uint8_t tick_parsed = 0;

/* Dirty-hack, but saves memory and runs on each platworm.
 * We will use pointers to the two previous vars for
 * "flag" pointers to check if timer list element is in
 * main queue or in fast queue (or NULL if is not in queue)
 */
#define IN_MAIN (tmgr_timer_t *) (&sys_uptime)
#define IN_FAST (tmgr_timer_t *) (&tick_parsed)

static volatile tmgr_timer_t *fast_queue = NULL;
static volatile tmgr_timer_t *main_queue = NULL;

/**
 * @section Task Manager core
 * @{
 */

void tmgr_interrupt(void)
{
        sys_uptime++;
        tick_parsed = 0;
}

void tmgr_process(void)
{
        if (tick_parsed) /* this is to save time for interrupts if we had just parsed queues */
                return;

        /* Full function is atomic; accept it as it is */
        ANTARES_DISABLE_IRQS();

        /* Check fast queue */
        if (fast_queue != NULL) {
                /* Save fast_queue list and make it ready for next timers.
                 * Also, prepare main_queue for insertions.
                 */
                tmgr_timer_t *fast = (tmgr_timer_t *) fast_queue,
                             *current = (tmgr_timer_t *) main_queue,
                             *previous = NULL;

                fast_queue = NULL;

                while (fast != NULL) {
                        while (current != NULL && current->expires < fast->expires) {
                                previous = current;
                                current = current->next;
                        }

                        if (!previous) { /* if new element is root */
                                main_queue = fast;
                                fast->prev = IN_MAIN;
                                
                                fast = fast->next; /* shift fast queue */

                                main_queue->next = current;
                                
                                if (current)
                                        current->prev = (tmgr_timer_t *) main_queue;

                        } else { /* new element is NOT root */
                                previous->next = fast;
                                fast->prev = previous;

                                fast = fast->next; /* shift fast queue */

                                if (current) { /* if our element is NOT last */
                                        previous->next->next = current;
                                        current->prev = previous->next;
                                } else { /* if our element IS last */
                                        previous->next->next = NULL;
                                }

                        }
                        
                        current = (tmgr_timer_t *) main_queue;                                
                        previous = NULL;
                }
        }

        /* Run main queue tasks */
        if (main_queue != NULL) {
                while (main_queue != NULL && main_queue->expires <= sys_uptime) {
                        tmgr_timer_t *current = (tmgr_timer_t *) main_queue;
                        main_queue = main_queue->next;
                        
                        void (*func)(uint8_t *) = current->func;
                        uint8_t *data = current->data;
                        current->prev = NULL;
                        current->next = NULL;

                        if (main_queue)
                                main_queue->prev = IN_MAIN; /* dummy pointer-flag */

                        ANTARES_ENABLE_IRQS(); /* run of timer function is no atomic, save interrupts! */
                        func(data);
                        ANTARES_DISABLE_IRQS();
                }
        }
        ANTARES_ENABLE_IRQS();
}


void tmgr_add_timer(tmgr_timer_t *timer)
{
        if (!timer)
                return;

        /* these lines are atomic */
        /* TODO!!! REPLACE WITH ANTARES_ATOMIC() {} TO SAVE STATE! */
        ANTARES_DISABLE_IRQS();
        timer->next = (tmgr_timer_t *) fast_queue;
        timer->prev = IN_FAST; /* set flag that this timer in fast queue */
        
        if (fast_queue)
                fast_queue->prev = timer;

        fast_queue = timer;
        ANTARES_ENABLE_IRQS();
}

void tmgr_del_timer(tmgr_timer_t *timer)
{
        if (!timer)
                return;
        
        /* these lines are atomic */
        /* TODO!!! REPLACE WITH ANTARES_ATOMIC TO SAVE STATE! */
        ANTARES_DISABLE_IRQS();
        if (timer->prev == IN_MAIN) {
                main_queue = timer->next;
                if (main_queue)
                        main_queue->prev = IN_MAIN;
        } else if (timer->prev == IN_FAST) {
                fast_queue = timer->next;
                if (fast_queue)
                        fast_queue->prev = IN_FAST;
        } else if (timer->prev) {
                timer->prev->next = timer->next;
        }
        ANTARES_ENABLE_IRQS();
}

void tmgr_mod_timer(tmgr_timer_t *timer, tmgr_uptime_t expires)
{
        if (!timer)
                return;

        /* these lines are atomic */
        /* TODO!!! REPLACE WITH ANTARES_ATOMIC() {} TO SAVE STATE! */
        ANTARES_DISABLE_IRQS();
        tmgr_del_timer(timer);
        timer->expires = expires;
        tmgr_add_timer(timer);
        ANTARES_ENABLE_IRQS();
}

tmgr_uptime_t tmgr_get_uptime(void)
{
        return sys_uptime;
}

void tmgr_delay(tmgr_uptime_t time)
{
        time += sys_uptime;
        while (time > sys_uptime)
                asm volatile ("nop");
}
