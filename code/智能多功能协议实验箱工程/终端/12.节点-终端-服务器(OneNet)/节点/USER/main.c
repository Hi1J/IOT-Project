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

	u8 buf[8];
	u16 illuvalu = 0;
	Usart1_Init(115200);
	printf("串口1初始化完成！\r\n");
	Zigbee_Init();
	DHT11_Config(1);
	ILLU_Gpio_Init();
	buf[0] = 0xfa;
	buf[1] = 0xff;
	buf[2] = 0x03;
	buf[7] = 0x00;
	while(1)
	{
		Get_DHT11_Data(&T,&H);
		illuvalu = ILLU_ReadData();
		
		buf[3] = T;//温度
		buf[4] = H;//湿度
		buf[5] = illuvalu >> 8;//光强高8位
		buf[6] = illuvalu & 0x00FF;//光强低8位

		Zigbee_Send_Data(buf,sizeof(buf));//发送数据
		delay_s(1);
	}

}