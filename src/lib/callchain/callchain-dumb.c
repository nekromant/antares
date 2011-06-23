#include <arch/antares.h>
#include <list.h>
#include <drivers/system/callchain-dumb.h>

void call_fuction_chain(struct fcall_chain_t* r)
{
  do
  {
    r->func();
  } while(r=r->next);
}


void call_function_add(struct fcall_chain_t* top, struct fcall_chain_t* ntail)
{
  //while(r=r->next);;
  
}
