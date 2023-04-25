#ifndef __USART_H_
#define __USART_H_

#include "stm32f10x.h"
#include "stdio.h"

#define SartBit 0xFAFC
#define RX_MAX 6
#define ON 1
#define OFF 0

void Usart1_Init(u32 b);
extern u8 RX_BUF[6];
extern u8 RX_OK;
extern u8 key;

#endif
