#include "tim6.h"

void TIM6_Delay_Init()
{
	RCC->APB1ENR |= 1 << 4;//TIM6时钟使能
	
	TIM6->CR1 |= 1 << 7;//自动重装载使能 （使用影子寄存器）
	
	TIM6->CR1 |= 1 << 3;//使用单脉冲模式
	
	TIM6->CR1 &= ~(1 << 2);//更新请求源
	
	TIM6->CR1 &= ~(1 << 1);//使能更新
	
	

}

void TIM6_Delay_ms(u16 ms)// 72000000MHZ/7200=10000HZ --- 0.1ms 
{
	TIM6->PSC = 7200 - 1;//预分频值为8400
	
	TIM6->ARR = ms * 10;//1ms计数10次
	
	TIM6->EGR |= 1 << 0;//将重装载值加载到影子中 （软件置1，硬件清零）
	
	TIM6->SR &= ~(1 << 0);//标志位清零
	
	TIM6->CR1 |= 1 << 0;//开启计数器
	
	while(!(TIM6->SR & (1 << 0)));//等待时间到 即SR自动置1 （硬件置1，软件清零）
	
	TIM6->SR &= ~(1 << 0);//标志位清零
	
	TIM6->CR1 &= ~(1 << 0);//关闭计数器
	
	
}

void TIM6_Delay_us(u16 us)// 72000000MHZ/72=1MHZ --- 1us 
{
	TIM6->PSC = 72 - 1;//预分频值为8400
	
	TIM6->ARR = us;//1us计数1次
	
	TIM6->EGR |= 1 << 0;//将重装载值加载到影子中 （软件置1，硬件清零）
	
	TIM6->SR &= ~(1 << 0);//标志位清零
	
	TIM6->CR1 |= 1 << 0;//开启计数器
	
	while(!(TIM6->SR & (1 << 0)));//等待时间到 即SR自动置1 （硬件置1，软件清零）
	
	TIM6->SR &= ~(1 << 0);//标志位清零
	
	TIM6->CR1 &= ~(1 << 0);//关闭计数器
	
	
}

void TIM6_Inter_Init(u16 psc,u32 arr)
{
	u32 Priority;
	
	RCC->APB1ENR |= 1 << 4;//使能时钟
	
	TIM6->CR1 |= 1 << 7;//自动重装载值使能
	
	TIM6->CR1 &= ~(1 << 3);//循环计算
	
	TIM6->CR1 &= ~(1 << 2);//使能更新
	
	TIM6->CR1 &= ~(1 << 1);//使能更新
	
	TIM6->DIER |= 1 << 0;//使能中断
	
	//中断设置
	NVIC_SetPriorityGrouping(7-2);
	Priority = NVIC_EncodePriority(7-2,1,2);
	NVIC_SetPriority(TIM6_IRQn , Priority);
	NVIC_EnableIRQ(TIM6_IRQn );
	
	TIM6->PSC = psc - 1;//预分配
	
	TIM6->ARR = arr;//自动重装载
	
	TIM6->EGR |= 1 << 0;//将重装载值加载到影子中 （软件置1，硬件清零）
	
	TIM6->CR1 |= 1 << 0;//使能
	
	
}
u8 K = 0;
void TIM6_IRQHandler(void)
{
	if(TIM6->SR & (1 << 0))
	{

		LED1_TOGGLE;  //LED1翻转
		K ++;  //标志位加1 500ms加一次 故5s加到10次
		TIM6->SR &= ~(1 << 0);//标志位清零
    

        
		}
		
}
