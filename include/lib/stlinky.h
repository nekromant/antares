#ifndef STLINKY_H
#define STLINKY_H


#define STLINKY_MAGIC 0xDEADF00D

/* NOTE: Since rxsize/txsize will be always one
   AXI transaction these will be hopefully atomic
   Therefore we use them as a dumb locking mechanism
 */

struct stlinky {
	uint32_t magic; /* [3:0] */
	unsigned char bufsize; /* 4 */
	char txsize; /* 5 */ 
	char rxsize; /* 6 */
	char reserved; /* 7 */
	char txbuf[CONFIG_LIB_STLINKY_BSIZE];  
	char rxbuf[CONFIG_LIB_STLINKY_BSIZE];
} __attribute__ ((packed));;

int stlinky_tx(volatile struct stlinky* st, const char* buf, int sz);

int stlinky_rx(volatile struct stlinky* st, char* buf, int sz);

void stlinky_wait_for_terminal(volatile struct stlinky* st);

int stlinky_avail(volatile struct stlinky* st);

extern volatile struct stlinky g_stlinky_term;

#endif

