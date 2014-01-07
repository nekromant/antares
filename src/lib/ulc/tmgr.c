#include <lib/tmgr.h>

static volatile tmgr_uptime_t sys_uptime = 0;
static volatile int32_t sys_fq = 1;
static volatile uint8_t tick_parsed = 0;

/* Dirty-hack, but saves memory and runs on each platworm.
 * We will use pointers to the two previous vars for
 * "flag" pointers to check if timer list element is in
 * main queue or in fast queue (or NULL if is not in queue)
 */
#define IN_MAIN (tmgr_timer_t *) (&sys_uptime)
#define IN_FAST (tmgr_timer_t *) (&sys_fq)

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
                           *current = (tmgr_timer_t *) main_queue;

                fast_queue = NULL;

                while (fast != NULL) {
                        while (current != NULL && current->expires < fast->expires) 
                                current = current->next;

                        if (!current) { /* only element in main queue */
                                fast->prev = IN_MAIN;
                                main_queue = fast;
                        } else if (!current->prev) { /* if next elem in queue is root */
                                current->prev = fast;
                                main_queue = fast;
                        } else {
                                current->prev->next = fast;
                                fast->prev = current->prev;
                                current->prev = fast;
                        }

                        fast = fast->next; /* shift fast queue, now current element detached from fast queue */
                        if (current)
                                current->prev->next = current;
                                
                }
        }

        /* Run main queue tasks */
        if (main_queue != NULL) {
                while (main_queue != NULL && main_queue->expires <= sys_uptime) {
                        void (*func)(uint8_t *) = main_queue->func;
                        uint8_t *data = main_queue->data;
                        main_queue = main_queue->next;
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

/**
 * @}
 *
 * @section Task Manager Uptime functions
 * @{
 */

tmgr_uptime_t tmgr_get_uptime(void)
{
        return sys_uptime;
}

void tmgr_delay(tmgr_uptime_t time)
{
        time += sys_uptime;
        while (time > sys_uptime);
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

/**
 * @}
 */
