#ifndef __CALLCHAIN_DUMB_H
#define __CALLCHAIN_DUMB_H
struct fcall_chain_t
{
  struct fcall_chain_t* next;
  void (*func)(void);
};


void call_fuction_chain(struct fcall_chain_t* r);
#endif