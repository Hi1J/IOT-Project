#ifndef __DRV_USART_H_
#define __DRV_USART_H_


#include "gd32f10x.h"
#include "stdio.h"

void usart0_init(uint32_t brr);

#define DEBUG_USART	USART0

#endif
