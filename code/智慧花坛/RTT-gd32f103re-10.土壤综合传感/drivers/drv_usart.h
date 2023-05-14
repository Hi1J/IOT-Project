#ifndef __DRV_USART_H_
#define __DRV_USART_H_


#include "gd32f10x.h"
#include "stdio.h"
#include "app.h"

extern rt_device_t u3_dev;
extern rt_uint8_t UART3_RX_BUF[64];
extern rt_uint16_t rs485_data_len;



void usart0_init(uint32_t brr);

void uart3_init(uint32_t brr);
void UART3_Send_Data(uint8_t *data,uint16_t len);


#define DEBUG_USART	USART0

#endif
