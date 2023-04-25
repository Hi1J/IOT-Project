#include "tim6.h"



void TIM6_Delay_Init(void)
{
	RCC->APB1ENR |= 1 << 4;//使能TIM6时钟
	
	TIM6->CR1 |= 1 << 7;//自动重装载使能（使用影子寄存器）
	
	TIM6->CR1 |= 1 << 3;//开启单脉冲模式
//	TIM6->CR1 &= ~(1 << 3);  //循环计数
	
	TIM6->CR1 &= ~(1 << 2);//更新请求源
	
	TIM6->CR1 &= ~(1 << 1);//使能更新
	
//	TIM6->CR1 &= ~(1 << 0);//关闭计数器
}



void TIM6_Delay_Ms(uint16_t ms)  //84MHZ / 8400 = 1000HZ -- 0.1 ms   
{
	TIM6->PSC = 8400 - 1;//预分频
	
	TIM6->ARR = ms * 10;//写入自动重装载值
	
	TIM6->EGR |= 1 << 0;//将重装载值加载到影子中 （软件置1，硬件清零）
	
	TIM6->SR &= ~(1 << 0);//标志位清零

	
	TIM6->CR1 |= 1 << 0;//使能计数器
	
	while(!(TIM6->SR & (1 << 0)));//等待时间到 即SR自动置1 （硬件置1，软件清零）

	
	TIM6->SR &= ~(1 << 0);//标志位清零

	
	TIM6->CR1 &= ~(1 << 0);//关闭计数器


}

void TIM6_Delay_Us(uint32_t us)  //84MHZ / 84 = 1MHZ -- 1us 
{
	TIM6->PSC = 84 - 1;//预分频
	
	TIM6->ARR = us;//写入自动重装载值
	
	TIM6->EGR |= 1 << 0;//将重装载值加载到影子中 （软件置1，硬件清零）
	
	TIM6->SR &= ~(1 << 0);//标志位清零

	
	TIM6->CR1 |= 1 << 0;//使能计数器
	
	while(!(TIM6->SR & (1 << 0)));//等待时间到 即SR自动置1 （硬件置1，软件清零）

	
	TIM6->SR &= ~(1 << 0);//标志位清零

	
	TIM6->CR1 &= ~(1 << 0);//关闭计数器


}


void TIM6_Inter_Init(u16 psc,u32 arr)
{
	u32 Priority;
	
	RCC->APB1ENR |= 1 << 4;
	
	TIM6->CR1 |= 1 << 7;
	
	TIM6->CR1 &= ~(1 << 3);
	
	TIM6->CR1 &= ~(1 << 2);
	
	TIM6->CR1 &= ~(1 << 1);
	
	TIM6->DIER |= 1 << 0;//使能中断
	
	//设置中断
	NVIC_SetPriorityGrouping(7-2);
	Priority = NVIC_EncodePriority(7-2,1,2);
	NVIC_SetPriority(TIM6_DAC_IRQn, Priority);
	NVIC_EnableIRQ(TIM6_DAC_IRQn);
	
	
	TIM6->PSC = psc - 1;
	
	TIM6->ARR = arr;
	
	TIM6->EGR |= 1 << 0;
	
	TIM6->CR1 |= 1 << 0;
	


}

void TIM6_DAC_IRQHandler(void)
{
	if(TIM6->SR & (1 << 0))
	{
		TIM6->SR &= ~(1 << 0);//标志位清零
		GPIOC->ODR ^= 1 << 5;//LED闪烁
	}

}


