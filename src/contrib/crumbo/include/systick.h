#ifndef _SYSTICK_H
#define _SYSTICK_H

#include <lib/callchain/callchain.h>
void systick_add(struct fcall_chain_t* f);

#define systick_counter_initsimple(seconds) \
systick_counter_set_limit(seconds*10);\
systick_counter_reset();

__inline void systick_counter_set_limit(int _climit);
__inline void systick_counter_reset();
__inline uint16_t systick_counter_getticks();
#endif