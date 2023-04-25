#include "tim9.h"

//TIM9的CH1通道对于的复用IO口为PA2
void TIM9_CH1_Init(u16 psc,u16 arr)
{
	/*配置GPIO的工作方式*/
	RCC->AHB1ENR |= 1 << 0;//开启GPIOA
	GPIOA->MODER &= ~(3 << (2 * 2));//清零
	GPIOA->MODER |= 2 << (2 * 2);//复用模式
	GPIOA->AFR[0] &= ~(15 << (4 * 2));//清零
	GPIOA->AFR[0] |= 3 << (4 * 2);//TIM9---AF3---0011
	
	/*配置TIM9计数器*/
	RCC->APB2ENR |= 1 << 16;//开启TIM9时钟
	TIM9->CR1 |= 1 << 7;//自动重装载值（使用影子寄存器）
	TIM9->CR1 &= ~(1 << 3);  //循环计数
	TIM9->CR1 &= ~(1 << 2);//更新请求源
	TIM9->CR1 &= ~(1 << 1);//使能更新
	
	TIM9->PSC = psc -1;
	TIM9->ARR = arr;
//	TIM9->CCR1 = ccre;//写入比较值
	TIM9->EGR |= 1 << 0;//将重装载值加载到影子中 （软件置1，硬件清零）
	
	/*配置PWM的工作方式*/
	TIM9->CCMR1 &= ~(1 << 7);//是计数器不受外部时钟源影响
	TIM9->CCMR1 |= 7 << 4;//4-6位配PWM的模式 111为模式2 当计数<比较值时无效
	TIM9->CCMR1 &= ~(1 << 3);//禁止与 TIMx_CCR1 相关的预装载寄存器。可随时向 TIMx_CCR1 写入数据（默认为0，可以不写）
	TIM9->CCMR1 &= ~(3 << 0);//通道1配置为输出模式
		 
	TIM9->CCER &= ~(1 << 1);//设置为高电平有效
	TIM9->CCER |= 1 << 0;//比较输出使能
	
	/*使能计数器*/
	TIM9->CR1 |= 1 << 0;

}