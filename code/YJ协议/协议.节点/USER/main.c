#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "DHT11.h"
#include "illu.h"
#include "string.h"
#include "Zigbee.h"

//Zigbee发送温湿光照强度
u8 T;
u8 H;

int main(void)
{

	u8 buf[16];
	u8 buf1[8];
	u16 illuvalu = 0;
	Usart1_Init(115200);
	printf("串口1初始化完成！\r\n");
	Zigbee_Init();
//	DHT11_Config(1);
//	ILLU_Gpio_Init();
	buf[0] = 0xfa;//起始位
	buf[1] = 0xff;//目标地址高位
	buf[2] = Zigbee_Address;//本机地址
	buf[3] = 0x03;//功能码
	buf[4] = 0x33;//温度
	buf[5] = 0x34;//湿度
	buf[6] = 0x35;//光强高8位
	buf[7] = 0x36;//光强低8位
	buf[8] = 0xfa;//起始位
	buf[9] = 0xff;//目标地址高位
	buf[10] = Zigbee_Address;//本机地址
	buf[11] = 0x03;//功能码
	buf[12] = 0x38;//温度
	buf[13] = 0x3a;//湿度
	buf[14] = 0x4a;//光强高8位
	buf[15] = 0x5a;//光强低8位
	Zigbee_Send_Data(buf,16);//发送数据
//	Zigbee_Send_Data(buf1,sizeof(buf));//发送数据
	while(1)
	{
//		Get_DHT11_Data(&T,&H);
//		illuvalu = ILLU_ReadData();


		
		delay_s(2);
	}

}