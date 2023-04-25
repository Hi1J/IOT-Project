#ifndef _HAL_WIRELESS_H_
#define _HAL_WIRELESS_H_


#include "stm32f10x.h"
#include "stdio.h"
//#include "crc16.h"
#include "delay.h"

/**********************底层中断接收数据**********************/
//定义无线一帧数据最大接收和发送数据个数
#define WIRELESS_RX_MAX 64
#define WIRELESS_TX_MAX 64
typedef struct
{
	uint8_t  rx_buff[WIRELESS_RX_MAX];//串口数据接收缓冲区
	uint8_t  tx_buff[WIRELESS_TX_MAX];//串口数据发送缓冲区
	uint8_t  rx_over;    //接收完成标志0 -- 没有接收完成 1 -- 接收完成
	uint32_t rx_count;                 //接收数据的个数
	uint32_t tx_count;                 //发送数据的个数
}WIRELESS_RX_TX;
extern WIRELESS_RX_TX wireless_data;


void UART4_Config(u32 boad);
void UART4_NVICConfig(void);
void UART_SendData(USART_TypeDef *puart,u8 *data,u16 len);
void Time4_Config(u16 psc,u16 arr);
#endif
