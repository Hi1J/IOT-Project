#ifndef __YJ_H_
#define __YJ_H_

//基本头文件--------
#include "stm32f10x.h"
#include "string.h"
#include "stdio.h"
//基本头文件--------

//用户头文件--------
#include "delay.h"
#include "Zigbee.h"
#include "mqtt.h"
//用户头文件--------


//用户可更改区------
#define Address  0xff //地址
#define Start_bit 0xfa  //起始位
#define Data_Frame_Max  8//数据帧最大值 
#define System_Frequency_MHz 72  //用户单片机主频（单位MHz）
//用户可更改区------


#define YJ_ON  1  
#define YJ_OFF 0 								
						

//连续数组链表创建
struct Yj_node{
	u8 YJ_RX_Buf[Data_Frame_Max];
	struct Yj_node* next;
};

//变量声明
extern uint8_t YJ_RX_Aframe_OK;
//extern uint8_t Multipe_data_frames_Flag;

extern struct Yj_node Yjnode1;
extern struct Yj_node Yjnode2;
extern struct Yj_node Yjnode3;

//函数声明
void YJ_Init(void);
void YJ_Tset(void);
void YJ_Data_reception(uint8_t data);
void YJ_Data_Analysis(void);
void YJ_TASK_START(void);
void YJ_TASK_END(void);
uint8_t YJ_TASK_STATUS(void);
//void YJ_Delay_us(uint16_t time);
void YJ_Delay_Ms(uint16_t time);








#endif
