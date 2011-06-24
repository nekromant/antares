#ifndef START_COND_H
#define START_COND_H

#define STARTPORT	PORTE
#define STARTDDR	DDRE
#define STARTPIN	PINE
#define STARTNUM	(1<<2)

void wait_start();
#endif