#ifndef _URPC_H
#define _URPC_H

/* 
 * We have 2 ways to work. 
 * a) Lookup methods in an array (fast, uses memory)
 * b) Do a linear search (slow, uses little memory).
 * With a dozen methods and an attiny2313 b is better. 
 * With ARM and a thousand methods... a is faster. 
 * The choice is yours to make
 */

/* if func is NULL - it's an event. else - it's a method */
/* Simple. isn't it? */

struct urpc_object
{
	const char* name;
	const char* type;
	void* (*func)(void*);
	const int alen; //arguments length
	const int rlen; //response length
	int id; /* internally used upon registration */
	int transportflags; /* any transport-specific flags */
	void* next;
};

enum urpc_states {
	URPC_ENABLED,
	URPC_DISABLED
};

/* enables event handling and processing */
void urpc_enable();
/* disables event handling and processing */
void urpc_disable();
/* exports an urpc method*/
void urpc_register(struct urpc_object* m);


#endif
