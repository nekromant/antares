#ifndef ARCH_DELAY_H
#define ARCH_DELAY_H

/* Just in case user forgot to */
#include <stdint.h>
#include <arch/antares.h>

void delay_4cycles(uint32_t c);

#define delay_cycles(c) delay_4cycles(c/4);
#define delay_s(s)  delay_4cycles(s * (get_system_clock() / 4))
#define delay_ms(s) delay_4cycles(s * (get_system_clock() / 1000 / 4))
#define delay_us(s) delay_4cycles(s * (get_system_clock() / 1000 / 1000 / 4))

#endif
