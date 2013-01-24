#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>
#include "stm32f10x_usart.h"

#define STDOUT_USART CONFIG_STM32_UARTIO_PORT
#define STDERR_USART CONFIG_STM32_UARTIO_PORT
#define STDIN_USART CONFIG_STM32_UARTIO_PORT

#ifndef STDOUT_USART
#define STDOUT_USART 1
#endif

#ifndef STDERR_USART
#define STDERR_USART 1
#endif

#ifndef STDIN_USART
#define STDIN_USART 1
#endif

int _write(int file, char *ptr, int len) {
    int n;
    switch (file) {
    case STDOUT_FILENO: /*stdout*/
        for (n = 0; n < len; n++) {
#if STDOUT_USART == 1
            while ((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET) {}
            USART1->DR = (*ptr++ & (uint16_t)0x01FF);
#elif  STDOUT_USART == 2
            while ((USART2->SR & USART_FLAG_TC) == (uint16_t) RESET) {
            }
            USART2->DR = (*ptr++ & (uint16_t) 0x01FF);
#elif  STDOUT_USART == 3
            while ((USART3->SR & USART_FLAG_TC) == (uint16_t)RESET) {}
            USART3->DR = (*ptr++ & (uint16_t)0x01FF);
#endif
        }
        break;
    case STDERR_FILENO: /* stderr */
        for (n = 0; n < len; n++) {
#if STDERR_USART == 1
            while ((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET) {}
            USART1->DR = (*ptr++ & (uint16_t)0x01FF);
#elif  STDERR_USART == 2
            while ((USART2->SR & USART_FLAG_TC) == (uint16_t) RESET) {
            }
            USART2->DR = (*ptr++ & (uint16_t) 0x01FF);
#elif  STDERR_USART == 3
            while ((USART3->SR & USART_FLAG_TC) == (uint16_t)RESET) {}
            USART3->DR = (*ptr++ & (uint16_t)0x01FF);
#endif
        }
        break;
    default:
        errno = EBADF;
        return -1;
    }
    return len;
}

int _read(int file, char *ptr, int len) {
    int n;
    int num = 0;
    switch (file) {
    case STDIN_FILENO:
        for (n = 0; n < len; n++) {
#if   STDIN_USART == 1
            while ((USART1->SR & USART_FLAG_RXNE) == (uint16_t)RESET) {}
            char c = (char)(USART1->DR & (uint16_t)0x01FF);
#elif STDIN_USART == 2
            while ((USART2->SR & USART_FLAG_RXNE) == (uint16_t) RESET) {}
            char c = (char) (USART2->DR & (uint16_t) 0x01FF);
#elif STDIN_USART == 3
            while ((USART3->SR & USART_FLAG_RXNE) == (uint16_t)RESET) {}
            char c = (char)(USART3->DR & (uint16_t)0x01FF);
#endif
            *ptr++ = c;
            num++;
        }
        break;
    default:
        errno = EBADF;
        return -1;
    }
    return num;
}


#if 0
int try_getc(int delay)
{
	int c;
	int timeout=delay;
#if STDOUT_USART == 1
	while (timeout && (!(USART1->SR & USART_FLAG_RXNE)))
#elif STDIN_USART == 2
	while (timeout && (!(USART2->SR & USART_FLAG_RXNE)))
#elif STDIN_USART == 3
	while (timeout && (!(USART3->SR & USART_FLAG_RXNE)))
#endif
	{
		timeout--;
		mdelay(1);
	}
#if STDOUT_USART == 1
	if ((USART1->SR & USART_FLAG_RXNE))
	{
		c = (char)(USART1->DR & (uint16_t)0x01FF);
#elif STDIN_USART == 2
	if ((USART2->SR & USART_FLAG_RXNE))
	{
		c = (char)(USART2->DR & (uint16_t)0x01FF);
#elif STDIN_USART == 3
	if ((USART3->SR & USART_FLAG_RXNE))
	{
		c = (char)(USART3->DR & (uint16_t)0x01FF);
#endif
		return (int) c;
	}
	return -1;
}
#endif
