#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>



void _sbrk_r( struct _reent *ptr, ptrdiff_t incr)
{
	char *ret;
	extern char _bss_end; // Defined by the linker
	static char *heap_end;
	char *prev_heap_end;


	if (heap_end == 0) {
		heap_end = &_bss_end;
	}
	prev_heap_end = heap_end;

	ets_uart_printf("SBRK: %p inc %x HEAP now @ %p\n", ptr, incr, heap_end);

/*
	char * stack = (char*) __get_MSP();


	errno = 0;

	if (heap_end + incr >  stack)
	{
		ets_uart_printf("Heap and stack collision\n");
		while(1);;
	}

	heap_end += incr;
*/

	
	return;
}

