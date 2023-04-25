#ifndef _HAL_WIRELESS_H_
#define _HAL_WIRELESS_H_


#include "stm32f10x.h"
#include "stdio.h"
#include "delay.h"

#define USART_DMA_COM  USART3

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

//相关操作功能码（低4位使用 -- 最多16位功能码使用）
//WDC--Wireless Data Communication 无线数据通信
#define WDC_CMD_    0x01//读多个寄存器 -- 所有传感器的数值和控制模块的状态
#define WDC_CMD_WriteReg     0x02//写多个寄存器 -- 可供控制模块的动作

#define BROADCAST_ADDR       0x00//广播地址
extern  uint8_t Slave_ID;        //从机本身地址


void USART3_Config(u32 boad);
void USART3_NVICConfig(void);
void UART_SendData(USART_TypeDef *puart,u8 *data,u16 len);
void Time4_Config(u16 psc,u16 arr);
void USART3_TxDMA_Config(void);
u8 USART3_DMA_SentData(char *p,uint32_t lenth);
uint8_t Slave_GetID(void);
#endif
