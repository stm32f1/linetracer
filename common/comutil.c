#include <string.h>
#include <stdarg.h>
#include "device.h"
#include "comutil.h"
#include "xbee.h"

struct comm_buf usart1 = {{0}, 0, 0};
struct comm_buf usart3 = {{0}, 0, 0};



uint8_t usart_transmit(USART_TypeDef *USARTx, uint8_t c)
{
	USARTx->DR = c;
	while ((USARTx->SR & USART_FLAG_TC) == 0);
	return c;
}



//int fdprintf(int fd, const char *format, ...)
//{
//	va_list arg;
//	int done;
//
//	va_start(arg, format);
//	done = vfdprintf(fd, format, arg);
//	va_end(arg);
//
//	return done;
//}



void USART1_IRQHandler(void)
{
	if (USART1->SR & USART_FLAG_RXNE) {
		USART1->SR &= (uint16_t)~USART_FLAG_RXNE;
		usart1.buf[usart1.len] = (uint8_t)USART1->DR;
		usart1.len++;
		if (usart1.len == MAX_COMM_BUFF)
			usart1.eol_flag = 1;
	}
}



void USART3_IRQHandler(void)
{
	if (USART3->SR & USART_FLAG_RXNE) {
		USART3->SR &= (uint16_t)~USART_FLAG_RXNE;
		usart3.buf[usart3.len] = (uint8_t)USART3->DR;
		usart3.len++;
		if (usart3.len == MAX_COMM_BUFF)
			usart3.eol_flag = 1;
		if (usart3.buf[usart3.len-1] == '\n') {
			usart3.eol_flag = 1;
			// lenが3であるかの確認は?
			proccess_received_data(usart3.buf);
			usart3.len = 0;
			usart3.eol_flag = 0;
		}
	}
}
