#ifndef HM_CHASSIS
#define HM_CHASSIS

#include <stm32f10x.h>

typedef enum{
  left = 0,
  right
} motor_t;

typedef enum{
  forward = 0,
  backward
} dir_t;

void motor_set_dir(motor_t motor, dir_t dir);
void motor_set_speed(motor_t motor, uint16_t speed);
void motor_stop(motor_t motor);

void chassis_write(dir_t dir1, dir_t dir2, uint16_t speed1, uint16_t speed2);
void chassis_stop(void);

#endif
