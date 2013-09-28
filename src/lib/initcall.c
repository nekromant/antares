#include <generic/initcall.h>

#define COMPONENT "init"
#ifdef CONFIG_LIB_INITCALL_DEBUG
#define DEBUG_LEVEL 4
#else 
#define DEBUG_LEVEL 0
#endif

#include <lib/printk.h>


/* That's ll we need here, really */

void initcall(struct init_object* o)
{
	if (!o->done) {
		dbg("running %s (%s)\n", o->name, o->type ? "low" : "high");
		o->initfunc();
		o->done++;
		dbg("'%s' complete\n", o->name);
	}
}

