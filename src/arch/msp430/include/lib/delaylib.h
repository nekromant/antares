#ifndef __DELAYLIB_H
#define __DELAYLIB_H

#define CYCLES_PER_S  (CONFIG_MSP430_DELAYLIB_FRQ)
#define CYCLES_PER_MS (CONFIG_MSP430_DELAYLIB_FRQ / 1000)
#define CYCLES_PER_US (CONFIG_MSP430_DELAYLIB_FRQ / 1000000)
#define CYCLES_PER_NS (CONFIG_MSP430_DELAYLIB_FRQ / 1000000000)

#ifdef CONFIG_MSP430_DELAYLIB_SCALING
void delay_scale(int mmul, int mdiv);
#else
#define delay_scale(mmul, mdiv);
#endif

void delay_s(int s);
void delay_ms(int ms);
void delay_us(int us);
void delay_ns(int ns);

#endif
