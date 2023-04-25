#include "stm32f4xx.h"
#include "led.h"
#include "systick.h"
#include "tim6.h"

//#include "systick.h"

int main(void)
{
	

	LED_Init();
	

	TIM6_Delay_Init();
	
	TIM6_Inter_Init(8400,5000);//500毫秒执行一次中断

	
	while(1)
	{
		
//		GPIOC->ODR |= 1 << 4;		
//		TIM6_Delay_Us(50000);
//		GPIOC->ODR &= ~(1 << 4);
//		TIM6_Delay_Us(50000);


		
	
	}
}

