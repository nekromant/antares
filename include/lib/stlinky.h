#ifndef __STLINKY_H
#define __STLINKY_H


#define STLINKY_MAGIC 0xDEADF00D

/* NOTE: This is a potentially racy thing. 
 * We have no way to atomically check and acquire a lock.
 * Only one read/write can be actually atomic here. 
 * So instead, we let mcu and host to put buffers
 * txsize/rxsize manage stuff
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

void stlinky_init(struct stlinky* st);

int stlinky_tx(volatile struct stlinky* st, char* buf, int sz);

int stlinky_rx(volatile struct stlinky* st, char* buf, int sz);

void stlinky_wait_for_terminal(volatile struct stlinky* st);

#endif

