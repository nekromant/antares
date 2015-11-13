#include <arch/antares.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <lib/stlinky.h>


volatile struct stlinky g_stlinky_term = {
	.magic = STLINKY_MAGIC,
	.bufsize = CONFIG_LIB_STLINKY_BSIZE,
	.up_tail = 0,
	.up_head = 0,
	.dw_tail = 0,
	.dw_head = 0
};


int stlinky_tx(volatile struct stlinky* st, const char* buf, int siz)
{
	//make local copy of tail and head
	int tail = st->up_tail;
	int head = st->up_head;

	//Figure out buffer usage
	int usage = head - tail;
	if (usage < 0)
		usage += CONFIG_LIB_STLINKY_BSIZE;

	//check if new data will fit
	if (usage + siz >= CONFIG_LIB_STLINKY_BSIZE)
		return 0;

	//copy in data (take care of possible split)
	int first_chunk = head + siz >= CONFIG_LIB_STLINKY_BSIZE ?
							CONFIG_LIB_STLINKY_BSIZE - head : siz;
	int second_chunk = siz - first_chunk;

	//Copy data
	memcpy((void *)&st->upbuf[head], buf, first_chunk);
	if(second_chunk > 0)
		memcpy((void *)&st->upbuf[0], buf + first_chunk, second_chunk);

	//increment head pointer
	st->up_head = (head + siz) % CONFIG_LIB_STLINKY_BSIZE;

	return siz;
}

int stlinky_rx(volatile struct stlinky* st, char* buf, int siz)
{
	//read head and tail values
	uint32_t tail = st->dw_tail;
	uint32_t head = st->dw_head;

	//return if empty
	if(head == tail)
		return 0;

	//read data
	int size_read = 0;
	uint32_t to_copy = 0;
	if(head > tail){
		to_copy = size_read + head - tail >= siz ? siz : size_read + head - tail;
		memcpy(buf, (void *)&st->dwbuf[tail], to_copy);
		size_read += to_copy;
	} else if(head < tail){
		to_copy = size_read + CONFIG_LIB_STLINKY_BSIZE - tail >= siz ?
							siz : size_read + CONFIG_LIB_STLINKY_BSIZE - tail;
		memcpy(buf, (void *)&st->dwbuf[tail], to_copy);
		size_read += to_copy;

		to_copy = size_read + head >= siz ?
								siz - size_read : head;
		memcpy(buf + size_read, (void *)st->dwbuf, to_copy);
		size_read += to_copy;
	}

	//move tail
	tail = (tail + size_read) % CONFIG_LIB_STLINKY_BSIZE;
	st->dw_tail = tail;

	return size_read;

}

int stlinky_avail(volatile struct stlinky* st)
{
	int32_t tail = st->dw_tail;
	int32_t head = st->dw_head;
	int32_t  data_available;

	//how much data there is in the ring buffer
	data_available = head - tail;
	if (data_available < 0)
		data_available += CONFIG_LIB_STLINKY_BSIZE;

	return data_available;
}

void stlinky_wait_for_terminal(volatile struct stlinky* st)
{
	stlinky_tx(st, "\n", 1);
	while(st->up_tail != st->up_head);
}

#ifdef CONFIG_LIB_STLINKY_NLIB

int _write(int file, char *ptr, int len) {
	return stlinky_tx(&g_stlinky_term, ptr, len);
}

int _read(int file, char *ptr, int len) {
	return stlinky_rx(&g_stlinky_term, ptr, len);
}

#endif
