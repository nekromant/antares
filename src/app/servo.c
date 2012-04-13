#define SMODE_START	0
#define SMODE_TRANSPORT	1
#define SMODE_HORIZ	2
#define SMODE_DROP	3


struct servo_front
{
	uint16_t open;
	uint16_t closed;
	uint16_t hold;
	int snum;
};

struct servo_mode
{
	int snum;
	uint16_t pos[10];
};

static struct servo_mode side[] = 
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
	
}