#include <arch/antares.h>

#define     LED0                  BIT0
#define     LED1                  BIT6
#define     LED_DIR               P1DIR
#define     LED_OUT               P1OUT


void init_leds(void) {
  LED_DIR |= LED0 + LED1;	//Set LED pins as outputs
  LED_OUT |= LED0 + LED1;	//Turn on both LEDs
}


ANTARES_APP(blink) {

  WDTCTL = WDTPW + WDTHOLD;	// Stop WDT
  /*Halt the watchdog timer
      According to the datasheet the watchdog timer
      starts automatically after powerup. It must be
      configured or halted at the beginning of code
      execution to avoid a system reset. Furthermore,
      the watchdog timer register (WDTCTL) is
      password protected, and requires the upper byte
      during write operations to be 0x5A, which is the
      value associated with WDTPW.*/

  init_leds();		//Setup LEDs

  BCSCTL3 |= LFXT1S_2;	//Set ACLK to use internal VLO (12 kHz clock)

  TACTL = TASSEL__ACLK | MC__UP;	//Set TimerA to use auxiliary clock in UP mode
  TACCTL0 = CCIE;	//Enable the interrupt for TACCR0 match
  TACCR0 = 11999;	/*Set TACCR0 which also starts the timer. At
				12 kHz, counting to 12000 should output
				an LED change every 1 second. Try this
				out and see how inaccurate the VLO can be */

  WRITE_SR(GIE);	//Enable global interrupts

  while(1) {
	//Loop forever, interrupts take care of the rest
  }
}

interrupt(TIMERA0_VECTOR) TIMERA0_ISR(void) {
  LED_OUT ^= (LED0 + LED1);	//Toggle both LEDs
}

