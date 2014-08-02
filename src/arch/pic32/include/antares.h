#ifndef ANTARES_H
#define ANTARES_H


/*
 * We use .initX sections for low and high init functions
 * APPS are called one by one in the main()
 */

/*
 * Since in a naked function we're pretty much limited
 * RTFM says:  The only statements that can be safely included in naked functions 
 * are asm statements that do not have operands. All other statements, including 
 * declarations of local variables, if statements, and so forth, should be avoided. 
 * Naked functions should be used to implement the body of an assembly function, while 
 * allowing the compiler to construct the requisite function declaration for the assembler. 
 * Thefore we place calls to the actual functions. That does the trick allowing us to 
 * achieve more
 */

#ifdef CONFIG_ANTARES_STARTUP


#define ANTARES_INIT_LOW(fn)						\
	void fn();							\
	__attribute__((naked))						\
	__attribute__((__section__(".init5"))) void fn ## _low(void) {	\
		fn();							\
	};								\
	void fn() 


#define ANTARES_INIT_HIGH(fn)						\
	void fn();							\
	__attribute__((naked))						\
	__attribute__((__section__(".init7"))) void fn ## _high(void) {	\
		fn();							\
	};								\
	void fn() 							\


#define ANTARES_APP(fn)							\
	void fn();							\
	__attribute__((naked))						\
	__attribute__((__section__(".init8"))) void fn ## _app(void) {	\
		fn();							\
	};								\
	void fn()							\
	


#define ANTARES_FINISH(fn)						\
	void fn();							\
	__attribute__((naked))						\
	__attribute__((__section__(".fini0"))) void fn ## _finish(void) { \
		fn();							\
	}								\
	void fn() 

#endif

#include <plib.h>
#include <generic/antares.h>
#define ANTARES_ENABLE_IRQS() INTEnableSystemMultiVectoredInt();
#define ANTARES_DISABLE_IRQS() INTDisableSystemMultiVectoredInt();


#endif
