#include "stm32f10x.h"
#include "delay.h"
#include "led.h"

//gpio流水灯
int main(void)
{
	u8 k = 0;
	LED_Init();
	
	
	while(1)
	{
//		if(k == 7)
//		{
//			LED1_7_Port->ODR = ~(0 << (k - 1));//第7灯关闭
//			k = 0;
//			LED8_Port->ODR = ~(1 << 13);//第8灯开启
//			delay_ms(300);
//			LED8_Port->ODR = ~(0 << 13);//第8灯关闭
//		}
//		
//		if(k == 7)k = 0;
//		LED1_7_Port->ODR = ~(1 << (k++));//1-7灯流水
//		LED8_Port->ODR = ~(1 << 13);
//		delay_ms(300);
		
		if(k == 7)
		{
			k = 0;
			LED1_7_Port->ODR = ~(0 << 6);//第7灯关闭
			LED8_Port->ODR = ~(1 << 13);//第8灯开启
		}
		else
		{
			LED1_7_Port->ODR = ~(1 << (k++));//1-7灯流水
			LED8_Port->ODR = ~(0 << 13);//第8灯关闭
		
		}
		delay_ms(300);//300ms延迟
	
	}
	
	

}
