#include <lib/tasks.h>

volatile uint32_t uptime = 0;
uint8_t num_handlers = 0;
handler_t * first = 0;

uint8_t add_handler(handler_t * data)
{
    // 0. The most simplest case - too late to run handler
    if(data->uptime < uptime) return TIME_OVER;
    // 1. The simplest case - no handlers. So, let's add the first one
    if(num_handlers == 0)
    {
        first = data;
    }
    // 2. The second case - there is only 1 handler.
    else if(num_handlers == 1)
    {
        #ifdef REM_HANDLER_IF_DEFINED
        if(first->handler == data->handler) // if this handler has been registered before
        {
            first->uptime = data->uptime;
            return;
        }
        #endif
        if(first->uptime > data->uptime) first->next = data;
        else
        {
            data->next = first;
            first = data;
        }
    }
    // 3. There are many handlers - run sorting algorythm
    else
    {
        uint8_t i = num_handlers;
        handler_t * current = first;
        handler_t * prev = 0;

        while(i != 0)
        {
            #ifdef REM_HANDLER_IF_DEFINED
            if(current->handler == data->handler) // remove this handler if registered
            {
                if(prev == 0) first = first->next;
                else prev->next = current->next;
                num_handlers--;
            }
            #endif
            if(current->uptime <= data->uptime)
            {
                data->next = current;
                if(prev == 0) first = data;
                else prev->next = data;
                i++;
                break;
            }
            prev = current;
            current = current->next;
            i--;
        }
        if(i==0) prev->next = data;
        #ifdef REM_HANDLER_IF_DEFINED
        else
        {
            while(i != 0)
            {
                if(current->handler == data->handler)
                {
                    prev->next = current->next;
                    num_handlers--;
                    break;
                }
                prev = current;
                current = current->next;
                i--;
            }
        }
        #endif
    }

    num_handlers++;
    return 0;
}

void task_manager(void)
{
    uint8_t i = num_handlers;
    handler_t * current = first;
    while(i != 0)
    {
        if(current->uptime == uptime)
        {
            current->handler();
        }
        else if(current->uptime < uptime) break;

        current = current->next;
        i--;
    }
    num_handlers -= i;
    uptime++;
}
