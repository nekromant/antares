#ifndef __CHASSIS_H
#define __CHASSIS_H


enum chs_actions
{
pawn,              
king,             
queen,             
left,              
right,             
l_t,               
l_b,               
r_t,               
r_b,               
};

/*
enum actions
{
move,
grip,
get_a_pawn,
release,void chassis_move_simple(char dir0, char dir1, int pwm0, int pwm1)
mid,
bot,
stdby,
stand,
open,
close,
mid2,
drop,
};
*/
void chassis_move_simple(char dir0, char dir1, int pwm0, int pwm1);
__inline void stop();
#endif