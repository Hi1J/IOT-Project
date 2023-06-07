#ifndef __USART_H_
#define __USART_H_

#include "gd32f10x.h"
#include "stdio.h"


#define UART3_RX_OK    1
#define UART3_RX_NO    0



void usart0_init(uint32_t brr);

void usart1_init(uint32_t brr);

void USART1_Send_A_Data(uint8_t data);

void USART1_Send_Data_buf(uint8_t *data,uint16_t len);

void usart2_init(uint32_t brr);

void USART2_Send_Data(uint8_t *data,uint16_t len);

void uart3_init(uint32_t brr);

void UART3_Send_Data(uint8_t *data,uint16_t len);


extern uint8_t UART3_RX_BUF[128];

extern uint16_t UART3_RX_LEN;

extern uint8_t UART3_RX_FLAG;

extern int TSET_INTERRUPT;

extern uint16_t FS_Ver;

extern volatile unsigned char u2_ok;


#endif
