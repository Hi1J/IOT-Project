#include "stm32f4xx.h"
#include "led.h"
#include "usart1.h"
#include "adc.h"

void delay(uint32_t t)
{
	while(t--);
	
}


int main(void)
{
	float temp = 0;
	float Vx = 0;

	Usart1_Init(115200,84,0);//波特率 115200 AHB2频率 84MHz 过采样模式 16倍过采样
//	ADC1_CH4_Init();
	printf("OK\r\n");
	while(1)
	{
		
//		temp = Get_ADC1_CH4();
//		Vx = (float)(temp * 3.3) / 4096;
//		
//		printf("数字量：%1.f\r\n模拟量：%1.f\r\n",temp,Vx);
//	printf("okok！\r\n");
		Usart1_Send_String((u8 *)"okok\r\n");
		delay(5000000);	
	
	}
}

