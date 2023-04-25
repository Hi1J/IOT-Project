#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "lcd.h"
#include "usart.h"
#include "jpg_app.h"
#include "fatfs_app.h"
#include "Zigbee.h"
#include "esp8266.h"
#include "mqtt.h"




//节点（Zigbee）---（温湿光照度）-->终端----（ESP8266）-->OneNet---(下发命令)--->LED灯亮灭
int main(void)
{

	Usart1_Init(115200);
	printf("串口1初始化成功\r\n");
	YJ_Init();//YJ协议初始化
	LED_Init();//LED初始化
	LCD_Init();//lcd初始化
	SD_Fatfs_Init();//sd卡初始化
	
	LCD_Clear(LIGHTBLUE);
	JPG_ShowFunc("0:/pic2/cat2.jpg",0,0);
	JPG_ShowFunc("0:/pic1/tem03.jpg",5,160);
	JPG_ShowFunc("0:/pic1/hem03.jpg",5,220);
	JPG_ShowFunc("0:/pic1/light03.jpg",5,280);
	LCD_ShowStr(55,160+9,(u8 *)"温度:");
	LCD_ShowStr(55,220+9,(u8 *)"湿度:");
	LCD_ShowStr(55,280+9,(u8 *)"光强:");
	
	LCD_ShowStr(125,160+9,(u8 *)"摄氏度");
	LCD_ShowStr(125,220+9,(u8 *)"RH%");
	LCD_ShowStr(140,280+9,(u8 *)"Lux");
	Zigbee_Init();//Zigbee初始化
	printf("Zigbee初始化完成！\r\n");
	Init_Esp8266();//ESP8266初始化
	printf("ESP8266初始化完成！\r\n");
	AliIoT_Parameter_Init();//OneNet参数初始化
	MQTT_ConectPack();//连接服务器
	delay_s(1);
	MQTT_Subscribe(S_TOPIC_NAME,0);//上传主题
	printf("OneNet初始化完成！\r\n");
	WiFi_Rx_Flag = 0;
	memset(WiFi_Rx_Buff,0,sizeof(WiFi_Rx_Buff));
	LED1_ON;
	
	MQTT_RxDataInPtr=MQTT_RxDataBuf[0]; 				 //指向发送缓冲区存放数据的指针归位
	MQTT_RxDataOutPtr=MQTT_RxDataInPtr; 				 //指向发送缓冲区读取数据的指针归位
	MQTT_RxDataEndPtr=MQTT_RxDataBuf[R_NUM-1];		     //指向发送缓冲区结束的指针归位
	
	MQTT_CMDInPtr=MQTT_CMDBuf[0];                        //指向命令缓冲区存放数据的指针归位
	MQTT_CMDOutPtr=MQTT_CMDInPtr;                        //指向命令缓冲区读取数据的指针归位
	MQTT_CMDEndPtr=MQTT_CMDBuf[C_NUM-1];              	 //指向命令缓冲区结束的指针归位
	
	while(1)
	{
		Zigbee_Data_Analysis();//Zigbee数据处理
		if(WiFi_Rx_Flag == 1)
		{
			memcpy(&MQTT_RxDataInPtr[2], WiFi_Rx_Buff, Rev_Len);  //拷贝数据到接收缓冲区
			MQTT_RxDataInPtr[0] = Rev_Len/256;                  	//记录数据长度高字节
			MQTT_RxDataInPtr[1] = Rev_Len%256;					 	//记录数据长度低字节
			MQTT_RxDataInPtr += RBUFF_UNIT;                                	//指针下移
			if(MQTT_RxDataInPtr == MQTT_RxDataEndPtr)                     	//如果指针到缓冲区尾部了
				MQTT_RxDataInPtr = MQTT_RxDataBuf[0];                    	//指针归位到缓冲区开头
			Rev_Len = 0;                                        	//串口5接收数据量变量清零
			WiFi_Rx_Flag = 0;
		
		}
		if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr)
		{
			printf("接收到数据：\r\n");
			if(MQTT_RxDataOutPtr[2] == 0x30)
			{
				printf("服务器等级0推送\r\n");
				MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);
			}
			MQTT_RxDataOutPtr += RBUFF_UNIT;                //指针下移
			if(MQTT_RxDataOutPtr == MQTT_RxDataEndPtr)      //如果指针到缓冲区尾部了
			{
				MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];      //指针归位到缓冲区开头              
			}	
		}
		if(MQTT_CMDOutPtr != MQTT_CMDInPtr)
		{
			printf("命令:%s\r\n",&MQTT_CMDOutPtr[2]);
			if(!memcmp(&MQTT_CMDOutPtr[2],"LED1ON",strlen("LED1ON")))
			{LED1_ON;}//LED1亮
			else if(!memcmp(&MQTT_CMDOutPtr[2],"LED1OFF",strlen("LED1OFF")))
			{LED1_OFF;}//LED1灭
			else printf("未知指令\r\n");
			
			MQTT_CMDOutPtr += CBUFF_UNIT;				//指针下移
			if(MQTT_CMDOutPtr == MQTT_CMDEndPtr)	    //如果指针到缓冲区尾部了
			MQTT_CMDOutPtr = MQTT_CMDBuf[0];			//指针归位到缓冲区开头	
		}			
	}

}


//发送数据函数
void send_data(uint8_t tem,uint8_t hum,uint16_t light)
{
	static char head[3];
	static int	dataLen = 0;			//报文长度
	static char temp[50];				//定义一个临时缓冲区1,不包括报头
	static char tempAll[100];			//定义一个临时缓冲区2，包括所有数据
	
	memset(head,0,3);
	memset(temp,0,50);
	memset(tempAll,0,100);
	
	sprintf(temp,"{\"tem\":\"%d\",\"hum\":\"%d\",\"light\":\"%d\"}",tem,hum,light);
	
	head[0] = 0x03;//固定报头
	head[1] = 0x00;//固定报头
	head[2] = strlen(temp);//剩余长度	
	
	sprintf(tempAll,"%c%c%c%s",head[0],head[1],head[2],temp);
	
	dataLen = strlen(temp) + 3;
	
	MQTT_PublishQs0(Data_TOPIC_NAME,tempAll, dataLen);//发送数据给服务器
}
