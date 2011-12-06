#include <arch/antares.h>
#include "include/memorizer.h"


static uint8_t* pos;
static int cache_pos;
static char cache;



static inline void eeprom_sync()
{
  eeprom_write_byte(pos++,cache);
  cache=0;
  cache_pos=0;
  pos++;
}

char action_add(char act)
{
  act &= 0x3; // two bits matter;
  cache |= act << cache_pos; //store in cache
  cache_pos+=2; 
  if (cache_pos>=8) 
      eeprom_sync();
  
}



/* 
 * Finalizes action list, synchronizes to eeprom 
 * Next call to action_add will start from the beginning
 * of the memory
 */
void action_finalize()
{
  
  action_add(ACT_END);
  if (cache_pos) eeprom_sync();
  pos = (uint8_t*) 0;
  cache_pos=8; //invalidate cache
}


/*
 * Pops the next action from the memory and advances pointer
 */
char action_pop()
{
  char ret;
  if (cache_pos>=8) 
    {
    cache = eeprom_read_byte(pos++);
    cache_pos=0;
    }
   ret = (cache >> cache_pos) & 0x3;
   cache_pos+=2;
   return 2;
}


