#ifndef STLINKY_H
#define STLINKY_H

#include <stdint.h>

#define STLINKY_MAGIC 0xDEADF00D

struct stlinky {
	uint32_t magic;
	uint32_t bufsize;
	uint32_t up_tail;
	uint32_t up_head;
	uint32_t dw_tail;
	uint32_t dw_head;
	char upbuf[CONFIG_LIB_STLINKY_BSIZE];
	char dwbuf[CONFIG_LIB_STLINKY_BSIZE];
} __attribute__ ((packed));


int stlinky_tx(volatile struct stlinky* st, const char* buf, int sz);

int stlinky_rx(volatile struct stlinky* st, char* buf, int sz);

void stlinky_wait_for_terminal(volatile struct stlinky* st);

int stlinky_avail(volatile struct stlinky* st);

extern volatile struct stlinky g_stlinky_term;

#endif

