#include "stm32f4xx.h"
#include "led.h"
#include "usart1.h"

void delay(uint32_t t)
{
	while(t--);
	
}


int main(void)
{
	
	uint8_t rec;//用于存放接收的数据
//	LED_Init();
	Usart1_Init(115200,84,0);//波特率 115200 AHB2频率 84MHz 过采样模式 16倍过采样

	
	while(1)
	{
		while(!(USART1->SR & (1 << 5)));//等待接收完成
		rec = USART1->DR;
		USART1->DR = rec;
		while(!(USART1->SR & (1 << 7)));//等待发送完成
		

//中断		
//		if(Rec)
//		{
//			
//			USART1->DR = Rec;
//			while(!(USART1->SR & (1 << 7)));//等待发送完成
//			Rec = 0;
//		}
//		
		
			
	
	}
}

