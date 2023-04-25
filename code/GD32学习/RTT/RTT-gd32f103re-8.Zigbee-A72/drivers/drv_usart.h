#ifndef __DRV_USART_H_
#define __DRV_USART_H_


#include "gd32f10x.h"
#include "stdio.h"
#include <rtthread.h>
#include <rtdbg.h>
#include "app.h"

extern rt_device_t u1_dev;

extern uint8_t USART1_RX_BUF[128];

void usart0_init(uint32_t brr);
void usart1_init(uint32_t brr);
void USART1_Send_Data(uint8_t *data,uint16_t len);

#define DEBUG_USART	USART0

#endif
