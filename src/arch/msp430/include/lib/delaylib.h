#ifndef __DELAYLIB_H
#define __DELAYLIB_H

void delay_s(unsigned long int s);
void delay_ms(unsigned long int s);

#ifndef CONFIG_MSP430_DELAYLIB_DYNAMIC
 #define delay_us(m) __delay_cycles(m * (CONFIG_MSP430_DELAYLIB_FRQ / 1000000UL) * CYCLES_PER_US);
 #define delaylib_set_clock(f) { }
#else
 void delaylib_set_clock(unsigned long int f);
 void delay_us(unsigned long int s);
#endif

#ifdef CONFIG_MSP430_DELAYLIB_SUMMARY
void delaylib_print_summary();
#else
#define delaylib_print_summary() { }
#endif

#endif
