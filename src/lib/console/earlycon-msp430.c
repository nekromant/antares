#include <stdio.h>
#include <lib/earlycon.h>
#include <lib/printk.h>

#define BAUDRATE 9600UL


static void msp430_init()
{
/* MSP430EXP is hardwired for 9600. Sucks */
        P2SEL1 |= BIT0 + BIT1;
        P2SEL0 &= ~(BIT0 + BIT1);
        UCA0CTL1 |= UCSWRST; 
        UCA0CTL1 = UCSSEL_3 ;                      // Set ACLK = 16M as UCBRCLK
        UCA0BR0 = F_CPU/(BAUDRATE*16);                              // 9600 baud
        UCA0MCTLW |= UCOS16;                      // 32768/9600 - INT(32768/9600)=0.41
        // UCBRSx value = 0x53 (See UG)  
        UCA0BR1 = 0;            
        UCA0CTL1 &= ~UCSWRST;                     // release from reset
        delaylib_set_clock(F_CPU);
}
	
}


struct early_console g_early_console = {
	.rxchar = lpad_getchar,
	.txchar = lpad_putchar,
	.init = lpad_init,		
	.havechar = lpad_avail,
};
