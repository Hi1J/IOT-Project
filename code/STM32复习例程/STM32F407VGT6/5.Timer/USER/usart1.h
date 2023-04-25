#ifndef __USART1_H
#define __USART1_H

#include "stm32f4xx.h"
#include "stdio.h"


extern u8 Rec;

void Usart1_Init(uint32_t Baud,uint32_t fclk,uint8_t OVER8);
void Usart1_Send_String(uint8_t * str);
int fputc(int ch,FILE *p);
void USART1_IRQHandler(void);


#endif

