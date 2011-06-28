#include <arch/antares.h>
#include <contrib/crumbo/adc.h>

static uint16_t limits[] = {
  200,
  270,
  270,
  350,
  380,
  370,
  300,
  300
};

static int _group_none[] =
{
  0  //count
};

static int _group_fwd[] =
{
  3,  //count
  4,5,7 //nums
};

static int _group_rht[]=
{
  2,
  7,0
};

static int _group_lft[]=
{
  2,
  4,6
};

static int _group_bck[] =
{
  2, 
  1,2
};

static int* groups[] =
{
  _group_none,
  _group_fwd,
  _group_rht,
  _group_lft,
  _group_bck
};

int sgroup;
__inline void odct_set_active_group(int _group)
{
  sgroup=_group;
}

static volatile char prevstate;

__inline void odct_get_collision_state()
{
  return prevstate;
}

__inline void process_dithered(volatile uint16_t* data)
{
  int i;
  char colstate=0;
  for (i=1;i<=groups[sgroup][0];i++)
  {
    if (data[groups[sgroup][i]]>=limits[groups[sgroup][i]])
    {
      #if 0
      dump8(0x00);
      dump16(i);
      dump16(groups[sgroup][i]);
      dump16(data[groups[sgroup][i]]);
      dump16(limits[groups[sgroup][i]]);
      #endif
      colstate=1;
    }
  }
  
  if (colstate!=prevstate) collision_avoidance(colstate);
  prevstate=colstate;
}