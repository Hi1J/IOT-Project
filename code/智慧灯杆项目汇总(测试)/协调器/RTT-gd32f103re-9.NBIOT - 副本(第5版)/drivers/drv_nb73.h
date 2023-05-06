#ifndef __DRV_NB73_H_
#define __DRV_NB73_H_

#include "gd32f10x.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"
#include <rtthread.h>
#include "app.h"

#if defined ( __CC_ARM  )
#pragma anon_unions
#endif


/*函数重命名*/
#define NB73_RX_BUF USART2_RX_BUF

typedef enum{
	CMD,    //指令模式
	NET,    //NET透传模式
	COAP,   //COAP透传模式
} ENUM_WKMODTypeDef;

#define RX_BUF_MAX_LEN     300                     //最大接收缓存字节数
extern struct  STRUCT_USARTx_Fram                  //串口数据帧的处理结构体
{
	char  Data_RX_BUF [ RX_BUF_MAX_LEN ];
	
	union {
		__IO uint16_t InfAll;
		struct {
			__IO uint16_t FramLength       :15;          // 14:0 
			__IO uint16_t FramFinishFlag   :1;           // 15 
		} InfBit;
	}; 
	
} strNB73_Fram_Record;

extern struct  STRUCT_USARTx_Fram strNB73_Fram_Record;
extern uint8_t NB72_Receive_Buf[50];
extern uint8_t IOT_DATA_LEN;
extern int reg_addre1;
extern int reg_addre2;
extern rt_device_t NB73_dev;

bool NB73_IOT_Init(void);
void NB73_ExitUnvarnishSend(void);
bool NB73_UnvarnishSend(void);
void NB73_Rst(void);
bool NB73_WKMODSet(ENUM_WKMODTypeDef enumWKMOD);
bool NB73_Cmd(char * cmd,char * reply1,char * reply2,uint32_t waittime);
void NB73_Send_A_Data(uint8_t data);
void NB73_HANDLE_DATA(void);
bool NB73_MQTT_CHACK(void);

#endif
