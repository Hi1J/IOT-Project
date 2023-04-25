#include "stm32f4xx.h"
#include "led.h"
#include "systick.h"
#include "tim9.h"


//ºôÎüµÆ³ÌÐò
int main(void)
{
	
	u16 k = 0;
//	LED_Init();
	TIM9_CH1_Init(16800,200);//20ms
	

	
	while(1)
	{
	while(k <= 195)
	{
		TIM9->CCR1 = k;
		k+=5;
		delay_ms(20);
	}
	
	while(k >= 5 )
	{
		TIM9->CCR1 = k;
		k-=5;
		delay_ms(20);
	
	}


		
	
	}
}

