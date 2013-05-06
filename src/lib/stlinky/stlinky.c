#include <arch/antares.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <lib/stlinky.h>

void stlinky_init(struct stlinky* st)
{
	st->magic = STLINKY_MAGIC;
	st->bufsize = CONFIG_LIB_STLINKY_BSIZE;
}

size_t stlinky_tx(volatile struct stlinky* st, char* buf, size_t siz)
{
	size_t sz = min_t(size_t, CONFIG_LIB_STLINKY_BSIZE, siz);
	while(st->txsize != 0);;; 
	memcpy((char*) st->txbuf, buf, sz);
	st->txsize = (unsigned char) sz;
	return sz;
}

/* TODO: We loose data here if we read less than avaliable */ 
size_t stlinky_rx(volatile struct stlinky* st, char* buf, size_t siz)
{
	size_t ret;
	while(st->rxsize == 0);;; 
	size_t sz = min_t(size_t, (size_t) st->rxsize, siz);
	memcpy(buf, (char*) st->rxbuf, sz);
	ret = (size_t) (st->rxsize);
	st->rxsize = 0;
	return ret;
}

void stlinky_wait_for_terminal(volatile struct stlinky* st)
{
	st->txbuf[0]='\n';
	st->txsize=1;
	while(st->txsize != 0);;; 
}

#ifdef CONFIG_LIB_STLINKY_NLIB

static volatile struct stlinky sterm;

ANTARES_INIT_LOW(stlinky_terminal) {
	stlinky_init(&sterm);
}

int _write(int file, char *ptr, int len) {
        return (int) stlinky_tx(&sterm, ptr, len);
}

int _read(int file, char *ptr, int len) {
	return (int) stlinky_rx(&sterm, ptr, len);
}

#endif
