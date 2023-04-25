#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "lora.h"
#include "hal_wireless.h"
#include "DHT11.h"
#include "illu.h"
#include "string.h"

//LORA发送温湿光照强度

u8 T;
u8 H;

int main(void)
{

	u8 buf[4];
	u16 illuvalu = 0;
	Usart1_Init(115200);
	printf("串口1初始化完成！\r\n");
	Lora_Config();
	printf("Lora初始化成功！\r\n");
	DHT11_Config(1);
	ILLU_Gpio_Init();
	
	while(1)
	{
		Get_DHT11_Data(&T,&H);
		illuvalu = ILLU_ReadData();
//		printf("%d\r\n",illuvalu);
//		sprintf((char *)buf,"%2d%2d%4d",T,H,illuvalu);
		buf[0] = T;//温度
		buf[1] = H;//湿度
		buf[2] = illuvalu >> 8;//光强高8位
		buf[3] = illuvalu & 0x00FF;//光强低8位
////		for(u8 i=0;i<4;i++)
////		{
////			printf("%x ",buf[i]);
////		}
////		printf("\r\n");
		Lora_SendData(buf,sizeof(buf));//发送数据
		delay_ms(100);
	}

}