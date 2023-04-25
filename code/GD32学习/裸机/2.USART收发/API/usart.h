#ifndef __USART_H_
#define __USART_H_

#include "gd32f10x.h"
#include "stdio.h"

void usart0_init(uint32_t brr);

void usart1_init(uint32_t brr);

void USART1_Send_A_Data(uint8_t data);

void USART1_Send_Data_buf(uint8_t *data,uint16_t len);


#endif
