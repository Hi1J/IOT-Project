#ifndef __DRV_A72_H_
#define __DRV_A72_H_

#include "gd32f10x.h"
#include "stdio.h"
#include "string.h"
#include <rtthread.h>
#include "app.h"



#define ZIGBEE_A72_NODE 1          //终端节点
#define ZIGBEE_A72_COORDINATOR 0   //协调器

#if ZIGBEE_A72_NODE

#define A72_WAKE_UP_SET()     gpio_bit_reset(GPIOA,GPIO_PIN_1);
#define A72_WAKE_UP_RESET()   gpio_bit_set(GPIOA,GPIO_PIN_1);

#endif

/*函数名重定义*/
#define A72_Send_Data USART1_Send_Data
#define A72_RX_BUF USART1_RX_BUF
#define A72_Delay_ms TIM6_Delay_ms

/*起始结束位*/
#define A72_Sbit 0x55
#define A72_Ebit 0xAA

/*复位*/
#define RESET_Bytes 7  //指令字节个数
#define C_RESET 0x000B//命令
#define L_RESET 0x0000//长度
#define V_RESET 0x00//校验

/*读取地址*/
#define Read_Adder_Bytes 7 //指令字节个数
#define C_Read_Adder 0x01C0//命令
#define L_Read_Adder 0x0000//长度
#define V_Read_Adder 0x00//校验
#define L_Adder_LEN 8    //IEEE地址长度
#define S_Adder_LEN 2    //短地址长度

/*A72返回信息结构体定义*/
typedef struct
{
	uint8_t L_Adder[L_Adder_LEN];//IEEE
	uint8_t S_Adder[S_Adder_LEN];//短地址
	uint8_t Type_Device;//设备类型
	uint8_t Channel;//信道
	uint8_t N_nodes;//节点数量(协调器)
	uint8_t S_nodes;//节点状态(协调器)
	uint8_t S_Network;//入网状态(终端节点)
	uint8_t Sleep_Time;//休眠时间(终端节点)
	
}A72_Return_Value;

/*变量声明*/
extern A72_Return_Value _value;

/*函数声明*/
void A72_Init(void);
int A72_Read_Address(void);
void A72_RESET(void);

#endif
