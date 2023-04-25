#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "pm25.h"
#include "DHT11.h"

u16 tem1;
u16 hum1;

int main(void)
{
	u16 value = 0;
	Usart1_Init(115200);
	printf("串口1初始化完成！\r\n");
	DHT11_Config(1);
	while(1)
	{
		DHT11_Show_Usart(&tem1,&hum1);
		delay_s(1);
	}

}