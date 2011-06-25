#ifndef _CALLCHAIN_H
#define _CALLCHAIN_H

#ifndef AUTOCONF_INCLUDED
#include <generated/autoconf.h>
#warning "Autoconf not included - fixed"
#endif

#ifdef CONFIG_LIB_CALLCHAIN_SIMPLE
#include "callchain-dumb.h"
#endif 


#endif