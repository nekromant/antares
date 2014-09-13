#ifndef ARCH_ANTARES_H
#define ARCH_ANTARES_H


#ifdef CONFIG_ANTARES_STARTUP

struct antares_app {
	void (*func)(void);
	struct antares_app *next;
};

void antares_app_register(struct antares_app *app);


#define ANTARES_INIT_LOW(fn)				\
        void 	__attribute__((constructor (101))) fn()	

#define ANTARES_INIT_HIGH(fn)				\
        void 	__attribute__((constructor (102))) fn()		
	

#define ANTARES_APP(fn)                                                 \
        void fn();							\
	static struct antares_app fn ## _apps = {			\
		.func = fn,						\
	};								\
	ANTARES_INIT_HIGH(fn ## _app_register) {			\
		antares_app_register(&fn ## _apps);			\
	}								\
        void fn()							\
		

#endif


#define ANTARES_DISABLE_IRQS() 
#define ANTARES_ENABLE_IRQS() 

#define get_system_clock()    0
#define set_system_clock(clk) 

#include <generic/antares.h>


#endif
