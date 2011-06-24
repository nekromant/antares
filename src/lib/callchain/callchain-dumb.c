#include <arch/antares.h>
#include <lib/callchain/callchain.h>

void call_fuction_chain(struct fcall_chain_t* r)
{
  do
  {
    r->func();
  } while(r=r->next);
}


void call_function_add(struct fcall_chain_t* top, struct fcall_chain_t* ntail)
{
  while(top->next) top=top->next;
  top->next=ntail;
}
