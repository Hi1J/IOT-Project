#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "tim6.h"


//程序效果：led1 闪烁5次 然后停止闪烁5秒  5秒后接着闪烁  （前5sLED每隔500s翻转  后5sLED不翻转） 

int main()
{
	LED_Init();
	TIM6_Inter_Init(7200,5000);//500ms产生一次中断
	while(1)
	{
		if(K == 10)     //当K==10时 刚好闪烁5次
		{
			TIM6->CR1 &= ~(1 << 0);//关闭计数器
			K = 0;//标志位清零
			delay_s(5);//延时5s
			
			TIM6->CR1 |= 1 << 0;//开启计数器
			
		}
	
	}
	
	
}