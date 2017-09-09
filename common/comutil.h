#ifndef COMUTIL_H
#define COMUTIL_H

#include "device.h"
#include <stdio.h>

#define XBEE_FILENO 3
#define xbee_printf(...) dprintf(XBEE_FILENO, __VA_ARGS__)
// dprintfはGNUによる拡張

#define MAX_COMM_BUFF 32
struct comm_buf {
	uint8_t buf[MAX_COMM_BUFF];
	size_t len;
	uint8_t eol_flag;
	// '\n'が送られた時とbufが満杯になった時に1
	// それ以外0
};

extern struct comm_buf usart1;
extern struct comm_buf usart3;

uint8_t usart_transmit(USART_TypeDef *USARTx, uint8_t c);

#endif /* COMUTIL_H */
