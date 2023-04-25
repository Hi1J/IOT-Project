#ifndef __USART_H_
#define __USART_H_

#include "stm32f10x.h"
#include "stdio.h"
#include "new.h"

#define Zigbee_Address  0xff
extern u8 Rx_OK;
extern u8 rx_buf[8];


void Usart1_Init(u32 b);

#endif

