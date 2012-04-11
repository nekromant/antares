#ifndef TMGR_H
#define TASKS_H

#include "stm32f10x.h"

/** @defgroup Defines section
  * @{
  */

#define TIME_OVER 1 // do not change. hm... doesn't matter :)

/** @}
  */

/** @defgroup Config section
  * @{
  */
#ifndef TASKS_CONFIG
// Config: if defined, you can add only 1 handler for 1 function-listener
// Optimises the work of the task manager if you use it for delayed looped run
#define REM_HANDLER_IS_DEFINED

#endif
/** @}
  */


/** @defgroup About variables
  * @{
  */

// Number of registered handlers

// Typedef for handler data structure
typedef struct hndlr{
    void (*handler)(void); // address of handler function (void)
    uint32_t uptime; // uptime to run
    // private: address of the next structure in list
    struct hndlr * next;
} handler_t;

/** @}
  */

/** @defgroup About functions
  * @{
  */

// Returns 0 in normal case, or 1 (TIME_OVER constant) if system uptime is above than handler's start time
int tmgr_register(handler_t * data);   // NOTE: It's better if data structure declared in global scope
                                         // And, DO NOT CHANGE STRUCTURES OF REGISTERED HANDLERS!
                                         // TODO: remove this limitation

void tmgr_tick(void); // add this function to SysTick timer / another simple timer / just in forever loop :)


unsigned int tmgr_get_uptime();
/** @}
  */

void sleep_ticks(unsigned int ticks);

#endif // TASKS_H
