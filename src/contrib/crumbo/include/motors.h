#ifndef __MOTORS_H
#define __MOTORS_H

__inline void motor_set_speed(int num, uint8_t value);
__inline int motor_get_dir(int num);
__inline void motor_set_dir(int num, int dir);
__inline void motor_stop(int motor);
__inline void __stop();
__inline void stop();

#endif