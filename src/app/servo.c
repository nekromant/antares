#include "servo.h"
#include "mcortex-hw.h"

static struct servo_mode sides[] = 
{
	{
		.snum = 6,
		.pos = { 2150, 1000, 1500, 1950},
	},
	{
		.snum = 5,
		.pos = { 700, 2100, 1500, 100},
	},
}


void side_servo_mode(int s, int mode)
{
	mctx_set_servo_pwm(sides[s].snum, sides[s].pos[mode]);
};
