// TODO: Legal stuff goes here
// This file is automagically generated, dude
// I wouldn't touch that if I were you, since all the stuff you'll fix
// Will eventually go down the closet
#include <arch/antares.h>
#include <avr/pgmspace.h>
#include "include/uart.h"
static char rx_buf[128];
static char tx_buf[128];
static volatile int tx_head;
static volatile int rx_pos;
static volatile int tx_pos;
static int rx_expect;
static volatile char tx_state; //progmem || ram || idle?
static volatile char* pgm_ptr;
ANTARES_INIT_LOW(comm_uart_init)
{
    UBRR0=8;
    UCSR0B=UCSR0B| (1 << RXEN0)| (1 << TXEN0)| (1 << TXCIE0)| (1 << RXCIE0);
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);

} // void comm_uart_init()

void comm_tx_next_byte()
{
    if (tx_state==1) {
        char c = pgm_read_byte(pgm_ptr++);
        UDR0=c;
        if (!c) {
            tx_state=0;
            pgm_ptr=0;
            return;
        }
    } else {
        if (tx_pos!=tx_head) {
            UDR0=tx_buf[tx_pos];
            tx_pos=(tx_pos+1) & 127;
        } else {
            if (pgm_ptr) {
                tx_state=1;
                comm_tx_next_byte();
            }
            else tx_state=0;
        }
    }
} // comm_tx_next_byte()
ISR(USART0_TX_vect)
{
    comm_tx_next_byte();
}; //USART0_TX_vect
ISR(USART0_RX_vect)
{
    rx_buf[rx_pos++]=UDR0;
    if (rx_pos == rx_expect) {
        rx_pos=0;
        //handle_packet(rx_buf, rx_expect);
    };
}; //USART0_RX_vect
__inline void comm_expect_length(int length)
{
    rx_expect=length;
} // comm_expect_length
void comm_putc(char c)
{
    tx_buf[tx_head++]=c;
    tx_head&= 127;
    if ((UCSR0A & (1<<UDRE0))) comm_tx_next_byte();
} // void comm_putc(char c)
void comm_puts(char* text)
{
    while(*text) comm_putc(*text++);
} // void comm_puts(char* text)
void comm_sync()
{
    while((tx_state) || (tx_pos!=tx_head));;
} //
void comm_send_pgms(char* pgm)
{
    pgm_ptr=pgm;
    if ((!tx_state)) {
        comm_tx_next_byte();
    }
} // comm_send_pgms
