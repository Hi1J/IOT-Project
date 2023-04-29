#include "drv_nb73.h"

#define DBG_TAG "nb73"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

struct  STRUCT_USARTx_Fram strNB73_Fram_Record = { 0 };
uint8_t NB72_Receive_Buf[50];
uint8_t IOT_DATA_LEN = 0;
int reg_addre1 = 0x0000;
int reg_addre2 = 0x0003;

/*
函数功能：NB73初始化
参数：void
返回值：bool  0:失败  1:成功
备注：
*/
bool NB73_IOT_Init(void)
{
	
	rt_kprintf("退出透传模式\n");
	NB73_ExitUnvarnishSend();
	
	rt_kprintf("CoAP透传模式\n");
	if(NB73_WKMODSet(COAP) == false)
		return false;
	
	rt_kprintf("连接有人云\n");
	if(NB73_Cmd("AT+NCDP=117.60.157.137,5683","OK",NULL,2500) == false)
		return false;
	
	rt_kprintf("复位\n");
	NB73_Rst();
	
	return 1;
}
/*
函数功能：退出透传模式
参数：void
返回值：void
备注：
*/
void NB73_ExitUnvarnishSend (void)
{
	
	rt_thread_mdelay(1000);
	
	printf("+++");
	
	rt_thread_mdelay(1000);
	
	printf("a");
	
	rt_thread_mdelay(500); 
	
	
}

/*
函数功能：进入透传模式
参数：void
返回值：bool  0:失败  1:成功
备注：
*/
bool NB73_UnvarnishSend(void)
{
	if (!NB73_Cmd("AT+ENTM","OK",0,500))
		return false;
	
	return 1;
}
/*
函数功能：复位
参数：void
返回值：void
备注：
*/
void NB73_Rst(void)
{
	NB73_Cmd ("AT+S","OK","LoRa Start!",1000);   	
//	NB73_Cmd ("AT+Z","OK","LoRa Start!",1000);   	
	rt_thread_mdelay(500); 
}
/*
函数功能：工作模式选择
参数：enumWKMOD
返回值：bool  0:失败  1:成功
备注：参数选择：CMD:指令模式 NET:NET透传模式 COAP:COAP透传模式
*/
bool NB73_WKMODSet(ENUM_WKMODTypeDef enumWKMOD)
{
	switch (enumWKMOD)
	{
		case CMD:
			return NB73_Cmd("AT+WKMOD=CMD","OK",NULL,2500); 

		case NET:
		  return NB73_Cmd("AT+WKMOD=NET","OK",NULL,2500); 

		case COAP:
		  return NB73_Cmd("AT+WKMOD=COAP","OK",NULL,2500); 

		default:
		  return false;
  }
}
/*
函数功能：发送AT指令
参数：cmd：待发送的指令 reply1、reply2：期待的响应 waittime：等待响应的时间
返回值：bool  0:失败  1:成功
备注：
*/
bool NB73_Cmd(char * cmd,char * reply1,char * reply2,uint32_t waittime)
{  
	
	uint8_t MAX_Repeat = 5;//最大重复次数
	bool NB73_OK_Flag = false;
	
	while(--MAX_Repeat)
	{
		
		strNB73_Fram_Record .InfBit .FramLength = 0; //重新开始接收新的数据包

		printf("%s\r\n",cmd);

		if ((reply1 == NULL) && (reply2 == NULL))       //不需要接收数据
			return true;
		
		rt_thread_mdelay(waittime);                 //延时
		
		strNB73_Fram_Record .Data_RX_BUF [ strNB73_Fram_Record .InfBit .FramLength ]  = '\0';

	//	rt_kprintf("%s",strNB73_Fram_Record .Data_RX_BUF);
	  
		if (( reply1 != NULL ) && ( reply2 != NULL ))
			NB73_OK_Flag = ((bool)strstr(strNB73_Fram_Record .Data_RX_BUF,reply1) || 
							  (bool)strstr(strNB73_Fram_Record .Data_RX_BUF,reply2)); 
		
		else if(reply1 != 0)
			NB73_OK_Flag = ((bool)strstr(strNB73_Fram_Record .Data_RX_BUF,reply1));
		
		else
			NB73_OK_Flag = ((bool)strstr(strNB73_Fram_Record .Data_RX_BUF,reply2));
		
		if(NB73_OK_Flag != false)break;
		
	}
	
	if(MAX_Repeat > 0)
		return true;	
	else
		return false;
	

}
/*
函数功能：发送1个8位数据
参数：data：待发送的数据
返回值：bool  0:失败  1:成功
备注：
*/
void NB73_Send_A_Data(uint8_t data)
{
	usart_data_transmit(USART2,data);
}

/*
函数功能：IOT-NB73数据处理
参数：用户自定义 默认：void
返回值：用户自定义 默认：void
备注：该函数主要用于处理服务器发过来的数据
	  处理数据方式为用户自定义 
*/
void NB73_HANDLE_DATA(void)
{
	
	uint8_t i = 0;
	
	rt_kprintf("服务器发来数据:\n");
	for(i=0;i<IOT_DATA_LEN;i++)
		rt_kprintf("%x ",NB72_Receive_Buf[i]);
	
	rt_kprintf("\n");
	
	memset(NB72_Receive_Buf,0,20);
	
	IOT_DATA_LEN = 0;
}

