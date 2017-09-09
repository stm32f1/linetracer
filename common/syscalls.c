#include <sys/stat.h>
#include <sys/types.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include "comutil.h"


extern int errno;

extern void *_heap_start;
void *stack_ptr __asm("sp");


int _close(int file)
{
	return -1;
}


int _read(int file, char *ptr, int dir)
{
	return 0;
}


int _write(int file, char *ptr, int len)
{
	int i;
	char *p = ptr;
	USART_TypeDef* USARTx;
	switch (file) {
		case STDOUT_FILENO:
			USARTx = USART1;
			break;
		case XBEE_FILENO:
			USARTx = USART3;
		default:
			break;
	}
	for (i = 0; i < len; i++) {
		usart_transmit(USARTx, (uint8_t)*(p++));
	}
	return i;
}


void _exit(int status)
{
	while (1);
}


caddr_t _sbrk(int incr)
{
	static void *heap_end = NULL;
	void *prev_heap_end;

	if (heap_end == NULL)
		heap_end = &_heap_start;

	prev_heap_end = heap_end;
	if (heap_end + incr > stack_ptr) {
		errno = ENOMEM;
		return (caddr_t)-1;
	}

	heap_end += incr;

	return (caddr_t)prev_heap_end;
}


int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}


int _isatty(int file)
{
	return 1;
}


int _lseek(int file, int ptr, int dir)
{
	return 0;
}
