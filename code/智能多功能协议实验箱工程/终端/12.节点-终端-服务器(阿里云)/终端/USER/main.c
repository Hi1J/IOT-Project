#include "stm32f10x.h"
#include "delay.h"
#include "lcd.h"
#include "usart.h"
#include "jpg_app.h"
#include "fatfs_app.h"
#include "Zigbee.h"
#include "esp8266.h"
#include "mqtt.h"




//节点（Zigbee）---（温湿光照度）-->终端----（ESP8266）-->阿里云
int main(void)
{

	Usart1_Init(115200);
	printf("串口1初始化成功\r\n");
	YJ_Init();//YJ协议初始化
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
	Init_Esp8266();//ESP8266初始化
	AliIoT_Parameter_Init();//阿里云参数初始化
	MQTT_ConectPack();//连接服务器
	delay_s(1);
	MQTT_Subscribe(S_TOPIC_NAME,0);//上传主题
	printf("初始化完成！\r\n");
	
	while(1)
	{
		Zigbee_Data_Analysis();//Zigbee数据处理
	}

}
