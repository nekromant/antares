#include <arch/antares.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <lib/stlinky.h>


volatile struct stlinky g_stlinky_term = {
	.magic = STLINKY_MAGIC,
	.bufsize = CONFIG_LIB_STLINKY_BSIZE,
	.txsize = 0,
	.rxsize = 0
};


int stlinky_tx(volatile struct stlinky* st, const char* buf, int siz)
{
	int sz = min_t(int, CONFIG_LIB_STLINKY_BSIZE, siz);
	while(st->txsize != 0);;; 
	memcpy((char*) st->txbuf, buf, sz);
	st->txsize = (unsigned char) sz;
	return sz;
}

/* TODO: We loose data here if we read less than avaliable */ 
int stlinky_rx(volatile struct stlinky* st, char* buf, int siz)
{
	int ret;
	while(st->rxsize == 0);;; 
	int sz = min_t(int, (int) st->rxsize, siz);
	memcpy(buf, (char*) st->rxbuf, sz);
	ret = (int) (st->rxsize);
	st->rxsize = 0;
	return ret;
}

int stlinky_avail(volatile struct stlinky* st)
{
	return st->rxsize;
}

void stlinky_wait_for_terminal(volatile struct stlinky* st)
{
	st->txbuf[0]='\n';
	st->txsize=1;
	while(st->txsize != 0);;; 
}

#ifdef CONFIG_LIB_STLINKY_NLIB

int _write(int file, char *ptr, int len) {
        return stlinky_tx(&g_stlinky_term, ptr, len);
}

int _read(int file, char *ptr, int len) {
	return stlinky_rx(&g_stlinky_term, ptr, len);
}

#endif
