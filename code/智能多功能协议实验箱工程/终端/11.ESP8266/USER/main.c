#include "stm32f10x.h"
#include "delay.h"
//#include "lcd.h"
#include "usart.h"
//#include "jpg_app.h"
//#include "fatfs_app.h"
#include "esp8266.h"
#include "mqtt.h"





//Zigbee接收温湿光照强度
int main(void)
{

	Usart1_Init(115200);
	printf("串口1初始化成功\r\n");
//	LCD_Init();//lcd初始化
//	SD_Fatfs_Init();//sd卡初始化
	Init_Esp8266();
	AliIoT_Parameter_Init();
	MQTT_ConectPack();
	delay_s(1);
	MQTT_Subscribe(S_TOPIC_NAME,0);
	printf("初始化完成！\r\n");
	
	while(1)
	{
		
		
	}

}
