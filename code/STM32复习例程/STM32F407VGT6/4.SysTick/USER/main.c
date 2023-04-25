#include "stm32f4xx.h"
#include "led.h"

#include "systick.h"

int main(void)
{
	

	LED_Init();
	
	delay_Inter_ms(500);//不管程序实现什么内容 这里会全程每隔500ms做一次中断服务函数内容  当然 前提是往后不使用SysTick


	
	while(1)
	{
//		GPIOC->ODR &= ~(1 << 4);
//		delay_ms(500);
//		GPIOC->ODR |= 1<< 4;
//		delay_ms(500);


		
	
	}
}

