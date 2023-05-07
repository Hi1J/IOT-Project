#ifndef __DRV_UART_H
#define __DRV_UART_H

#include "gd32f10x.h"
#include "stdio.h"
#include <rtthread.h>
#include <rtdbg.h>
#include "app.h"

#define DEBUG_USART	USART0

//变量声明
extern rt_device_t u1_dev;

extern uint8_t USART1_RX_BUF[128];
extern uint8_t USART1_RX_OK;

//函数声明
void dbgusart_config(void);

void usart1_init(uint32_t brr);
void USART1_Send_Data(uint8_t *data,uint16_t len);

#endif /* __DRV_UART_H */
