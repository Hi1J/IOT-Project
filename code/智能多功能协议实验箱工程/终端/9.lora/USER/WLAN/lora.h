#ifndef _LORA_H_
#define _LORA_H_

#include "stm32f10x.h"
#include "delay.h"
#include "string.h"
#include "stdio.h"
#include "hal_wireless.h"
//#include "sensor.h"

//高四位代表M1 低四位代表M0
//M0 -- SPI_CS    -- PA8 (通用推挽输出模式)
//M1 -- SPI_SCK   -- PA11 (通用推挽输出模式)
//AUX - SPI_MISO  -- PA12 (浮空输入模式)
#define LORA_M0_CLK       RCC_APB2Periph_GPIOA
#define LORA_M0_PORT      GPIOA
#define LORA_M0_PIN       GPIO_Pin_8

#define LORA_M1_CLK       RCC_APB2Periph_GPIOA
#define LORA_M1_PORT      GPIOA
#define LORA_M1_PIN       GPIO_Pin_11

#define LORA_AUX_CLK       RCC_APB2Periph_GPIOA
#define LORA_AUX_PORT      GPIOA
#define LORA_AUX_PIN       GPIO_Pin_12

#define LORA_MODE_PATTERN     0x00 //一般模式
#define LORA_MODE_WARKUP      0x01 //唤醒模式
#define LORA_MODE_POWERSAVE   0x10 //省电模式
#define LORA_MODE_SLEEP       0x11 //休眠模式
/*********************设置Lora工作参数***********/
//修改参数命令
#define CMD_STOREPRM      0xC0//所设置参数会保存
#define CMD_NOTSTOREPRM   0xC2//所设置参数不会保存
//UART协议中奇偶校验
#define PRM_UART_Parity_8N1      0x00//不奇偶校验
#define PRM_UART_Parity_8O1      0x01//偶校验
#define PRM_UART_Parity_8E1      0x10//奇校验
//UART协议中波特率
#define PRM_UART_BaudRate_1200    0x00
#define PRM_UART_BaudRate_2400    0x01
#define PRM_UART_BaudRate_4800    0x02
#define PRM_UART_BaudRate_9600    0x03
#define PRM_UART_BaudRate_19200   0x04
#define PRM_UART_BaudRate_38400   0x05
#define PRM_UART_BaudRate_57600   0x06
#define PRM_UART_BaudRate_115200  0x07

//设置空中数据传输速率
#define PRM_Sky_Speed_0_3       0x00//0.3kbps
#define PRM_Sky_Speed_1_2       0x01//1.2kbps
#define PRM_Sky_Speed_2_4       0x02//2.4kbps
#define PRM_Sky_Speed_4_8       0x03//4.8kbps 
#define PRM_Sky_Speed_9_6       0x04//9.6kbps
#define PRM_Sky_Speed_19_2      0x05//19.2kbps

//设置传输模式
#define PRM_TRAN_MODE_TT        0x00//透传传输模式
#define PRM_TRAN_MODE_FP        0x01//定点传输模式
//AUX RX TX IO驱动方式
#define PRM_IO_Driver_PP        0x01//内部具有上拉
#define PRM_IO_Driver_OD        0x00//内部具有开路
//唤醒时间
#define PRM_WarkUp_Time_250     0x00//单位ms
#define PRM_WarkUp_Time_500     0x01
#define PRM_WarkUp_Time_750     0x02
#define PRM_WarkUp_Time_1000    0x03
#define PRM_WarkUp_Time_1250    0x04
#define PRM_WarkUp_Time_1500    0x05
#define PRM_WarkUp_Time_1750    0x06
#define PRM_WarkUp_Time_2000    0x07

//FEC纠错开关
#define PRM_FEC_OPEN            0x01//打开
#define PRM_FEC_CLOSE           0x00//关闭
//设置发射功率
#define PRM_EmissionPower_20    0x00//20dbm
#define PRM_EmissionPower_17    0x01
#define PRM_EmissionPower_14    0x02
#define PRM_EmissionPower_10    0x03

//设置默认的参数（也可以在外面设置参数）
#define CMD_DEFAULT                    CMD_STOREPRM
#define PRM_UART_Parity_DEFAULT        PRM_UART_Parity_8N1
#define PRM_UART_BaudRate_DEFAULT      PRM_UART_BaudRate_9600
#define PRM_Sky_Speed_DEFAULT          PRM_Sky_Speed_2_4
#define PRM_CHANNEL_DEFAULT            0x17//00-73h变化 410~525MHz
#define PRM_TRAN_MODE_DEFAULT          PRM_TRAN_MODE_TT
#define PRM_WarkUp_Time_DEFAULT        PRM_WarkUp_Time_250
#define PRM_FEC_DEFAULT                PRM_FEC_OPEN
#define PRM_EmissionPower_DEFAULT      PRM_EmissionPower_20

typedef struct
{
	u8 head;
	u8 addh;
	u8 addl;
	u8 sped;
	u8 chan;
	u8 option;
}LORA_WORKPRM;//工作参数

typedef union 
{
	LORA_WORKPRM lora_workprm;
	u8 buff[6];
}LORA_WORKDATA;//接收或则发送工作参数


void Lora_Config(void);              //Lora初始化
void Lora_SendData(u8 *data,u16 len);//Lora发送数据
void Lora_SendCmd(u8 *data,u16 len);
void Lora_SetMode(u8 mode);          //设置Lora模块的模式
void Lora_Reset(void);               //Lora模块进行复位
void Lora_CurrentPram(void);         //获取Lora模块当前的参数
void Lora_WorkParameters(void);      //显示Lora模块的参数
void Lora_SetWorkParameters(void);   //设置Lora模块的参数
void Lora_DataAnalysis(void);        //数据的解析

void DataAnalysis_Func00(void);       //功能码0x00的解析
void DataAnalysis_Func03(void);       //功能码0x03的解析
void DataAnalysis_Func04_SetFlag(void);//功能码0x04的解析
void DataAnalysis_Func04_TxAck(void);
#endif
