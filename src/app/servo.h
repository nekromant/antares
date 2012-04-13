#ifndef _SERVO_H
#define _SERVO_H

#define SMODE_START	0
#define SMODE_TRANSPORT	1
#define SMODE_HORIZ	2
#define SMODE_DROP	3


struct servo_mode
{
	int snum;
	uint16_t pos[10];
};

#endif