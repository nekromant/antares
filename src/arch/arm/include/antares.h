//Top level header
//This sucks in the mcu-headers
#ifndef _ANTARES_H
#define _ANTARES_H

#include <generic/macros.h>

#define ANTARES_DISABLE_IRQS() __disable_irq()
#define ANTARES_ENABLE_IRQS() __enable_irq()

/* 
 * normal fn(); call would cause optimizer to screw things up for 
 * no apparent reason. 
 * asm("bl " #fn); woould work though. So let it stay like that
 */

#ifdef CONFIG_ANTARES_STARTUP

#define ANTARES_INIT_LOW(fn)						\
	void fn();							\
	__attribute__((naked))						\
	__attribute__((__section__(".text.antares_init_low"))) void fn ## _low(void) { \
		asm("bl " #fn);						\
	};								\
	void fn() 

#define ANTARES_INIT_HIGH(fn)						\
	void fn();							\
	__attribute__((naked))						\
	__attribute__((__section__(".text.antares_init_high"))) void fn ## _high(void) { \
		asm("bl " #fn);						\
	};								\
	void fn() 							\


#define ANTARES_APP(fn)							\
	void fn();						\
	__attribute__((naked))						\
	__attribute__((__section__(".text.antares_app"))) void fn ## _app(void) { \
		asm("bl " #fn);						\
	};								\
	void fn()							\
	


#define ANTARES_FINISH(fn)						\
	void fn();						\
	__attribute__((naked))						\
	__attribute__((__section__(".antares_app_end"))) void fn ## _finish(void) { \
		asm("bl " #fn);						\
	}								\
	void fn() 						
#else

#define ANTARES_INIT_LOW(fn) void fn(void)
#define ANTARES_INIT_HIGH(fn) void fn(void)
#define ANTARES_APP(fn) void fn(void)

#endif

#endif
