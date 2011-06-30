#ifndef __CHASSIS_H
#define __CHASSIS_H


enum chs_actions
{
pawn,              
king,             
queen,             
left,              
right,             
};

void chassis_move_precise(int dir, uint8_t perfect_speed, uint8_t dist);
//void chassis_turn(int dir, uint8_t perfect_speed, int deg);
void chassis_turn(int dir, int pwm, int deg);
void reset_direction();
#endif