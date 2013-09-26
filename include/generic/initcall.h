#ifndef INITCALL_H
#define INITCALL_H

struct init_object {
	void (*initfunc)();
	int done; /* Initialized */
#ifdef CONFIG_LIB_INITCALL_DEBUG
	char* name; /* For debugging */
	int type; 
#endif
};


#ifndef CONFIG_LIB_INITCALL_DEBUG

#define ANTARES_INITCALL_LOW(func,...)			\
	void func();					\
	struct init_object initobject_ ## func = {	\
		.initfunc = func,			\
		.done = 0				\
	};						\
	ANTARES_INIT_LOW(__init_ ## func ) {		\
		initcall(& initobject_ ## func);	\
			}				\
	void func() 					\

#define ANTARES_INITCALL_HIGH(func,...)			\
	void func();					\
	struct init_object initobject_ ## func = {	\
		.initfunc = func,			\
		.done = 0				\
	};						\
	ANTARES_INIT_HIGH(__init_ ## func ) {		\
		initcall(& initobject_ ## func);	\
	}						\
	void func() 					\

#else

#define ANTARES_INITCALL_LOW(func,...)			\
	void func();					\
	struct init_object initobject_ ## func = {	\
		.initfunc = func,			\
		.done = 0,				\
		.type = 1,				\
		.name = #func				\
	};						\
	ANTARES_INIT_LOW(__init_ ## func ) {		\
		initcall(& initobject_ ## func);	\
			}				\
	void func() 					\

#define ANTARES_INITCALL_HIGH(func,...)			\
	void func();					\
	struct init_object initobject_ ## func = {	\
		.initfunc = func,			\
		.done = 0,				\
		.type = 1,				\
		.name = #func				\
	};						\
	ANTARES_INIT_HIGH(__init_ ## func ) {		\
		initcall(& initobject_ ## func);	\
	}						\
	void func() 					\

#endif


#define DEPENDS(dep)						\
	do {							\
		extern struct init_object initobject_ ## dep;	\
		initcall(&initobject_ ## dep);			\
	} while (0);

/** 
 * Initialise object. Noop if already initialized
 * 
 * @param o init_object to run
 */

void initcall(struct init_object* o);

#endif
