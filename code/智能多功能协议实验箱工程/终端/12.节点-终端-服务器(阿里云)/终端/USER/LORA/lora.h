#ifndef __LORA_H_
#define __LORA_H_

#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
#include "wlan_user.h"

#define M0(x) x?(GPIO_SetBits(GPIOA,GPIO_Pin_8)):(GPIO_ResetBits(GPIOA,GPIO_Pin_8))
#define M1(x)x?(GPIO_SetBits(GPIOA,GPIO_Pin_11)):(GPIO_ResetBits(GPIOA,GPIO_Pin_11))
#define AUX_Sta (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12))


extern u8 usart4_buf[128];
extern u8 Rx_OK;

void Init_USART4_Lora(uint32_t brr);
void Send_data_Usart4(uint8_t c);
void Send_Data_Lora(uint8_t *data ,uint8_t size);
void Send_String_lora(uint8_t *data);
void Init_Lora(void);


#endif