#ifndef __ANTARES_H
#define __ANTARES_H

#include <generic/macros.h>

#warning "ANTARES_INIT*/ANTARES_APP macros are not supported on this platform/compiler. You will have to supply your own main code"

#define ANTARES_INIT_LOW(fn) \
	void fn(void) {

#define ANTARES_INIT_HIGH(fn) \
	void fn(void) {

#define ANTARES_APP(fn) \
	void fn(void) {

#define ANTARES_FINISH(fn) \
	void fn(void) {

#endif
