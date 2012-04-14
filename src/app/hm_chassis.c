#include "hm_chassis.h"

#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>

void motor_set_dir(motor_t motor, dir_t dir)
{
  if(motor == left)
  {
    if(dir == forward)
    {
      GPIO_SetBits(GPIOB, GPIO_Pin_14);
      GPIO_ResetBits(GPIOB, GPIO_Pin_15);
    }
    else
    {
      GPIO_SetBits(GPIOB, GPIO_Pin_15);
      GPIO_ResetBits(GPIOB, GPIO_Pin_14);
    }
  }
  else
  {
    if(dir == forward)
    {
      GPIO_SetBits(GPIOB, GPIO_Pin_13);
      GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    }
    else
    {
      GPIO_SetBits(GPIOB, GPIO_Pin_12);
      GPIO_ResetBits(GPIOB, GPIO_Pin_13);
    }
  }
}

void motor_set_speed(motor_t motor, uint16_t speed)
{
  if(motor == left)
  {
    TIM_SetCompare1(TIM3, speed);
  }
  else
  {
    TIM_SetCompare2(TIM3, speed);
  }
}

void motor_stop(motor_t motor)
{
  if(motor == left)
  {
    GPIO_ResetBits(GPIOB, GPIO_Pin_14 | GPIO_Pin_15);
  }
  else
  {
    GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13);
  }
}

/////////////////////////////////////////////////////////////////

void chassis_write(dir_t dir1, dir_t dir2, uint16_t speed1, uint16_t speed2)
{
  motor_set_dir(left, dir1);
  motor_set_dir(right, dir2);
  motor_set_speed(left, speed1);
  motor_set_speed(right, speed2);
}

void chassis_stop(void)
{
  GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
}
