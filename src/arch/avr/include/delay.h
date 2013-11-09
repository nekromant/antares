#ifndef ARCH_DELAY_H
#define ARCH_DELAY_H

/* Good people who made avr-libc have done all the work for us
 * So, we just wrap it
 */

#include <util/delay.h>

#define delay_cycles(s) __builtin_avr_delay_cycles(s)
#define delay_s(s) _delay_ms(s * 1000)
#define delay_ms(s) _delay_ms(s)
#define delay_us(s) _delay_us(s)

#endif
