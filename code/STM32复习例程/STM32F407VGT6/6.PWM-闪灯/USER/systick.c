#include "systick.h"


void delay_ms(u32 ms)
{
	SysTick->CTRL &= ~(1 << 2); //时钟源选择外部时钟源
	
	SysTick->LOAD = SysTick_ * ms * 1000;//写入重装载值   = 外部时钟频率 * us 
	
	SysTick->VAL = 0;//计算器清零
	
	SysTick->CTRL |= 1 << 0; //使能计数器
	
	while(!(SysTick->CTRL & (1 << 16)));//等到减到0（判断标志位）
	
	SysTick->CTRL &= ~(1 << 0);//关闭计数器
	
		

}


//中断方式 
void delay_Inter_ms(u32 ms)
{
	SysTick->CTRL &= ~(1 << 2);//时钟源选择外部时钟源
	
	SysTick->LOAD =SysTick_ * ms * 1000;
	
	SysTick->VAL = 0;//计算器清零
	
	SysTick->CTRL |= 1 << 1; //开中断
	
	SysTick->CTRL |= 1 << 0;//使能计数器
	
	
	//这里不需要关中断 我们是需要每隔多少ms就执行中断服务函数 且 不放在主函数的while中
//	SysTick->CTRL &= ~(1 << 0);
	
	

}

//中断服务函数
//这里不需要设置NVIC 因为SysTick和NVIC同在内核中
void SysTick_Handler()
{
	if(SysTick->CTRL & (1 << 16))
	{
		//这里不需要清标志位，因为CTRL的第16位会自动硬件清零
		
		//中断函数内容
		GPIOC->ODR ^= 1 << 7;//led4翻转  
	
	
	}
}