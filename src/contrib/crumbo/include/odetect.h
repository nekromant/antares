#ifndef _ODETECT_H
#define _ODETECT_H

__inline void collision_avoidance(char state);
__inline void odct_set_active_group(int _group);
enum 
{
  group_fwd,
  group_rht,
  group_lft,
  group_bck
};
#endif