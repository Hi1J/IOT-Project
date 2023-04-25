#include "tim4.h"


void TIM4_CH3_Init(u16 psc,u16 arr)
{
	//配置GPIOB
	GPIO_InitTypeDef GPIO_InitStructure;
	//时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//配置TIM4
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//时钟不分割
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStructure.TIM_Period = arr;//自动重装载置
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc-1;//预分频
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	//配置PWM
	TIM_OCInitTypeDef TIM_OCInitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//CNT > CCR时输出有效电平
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//高电平有效
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//输出使能
	TIM_OC3Init(TIM4,&TIM_OCInitStructure);
	
	//使能预装载寄存器
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
	
	//使能
	TIM_Cmd(TIM4,ENABLE);
	
	
	
	
}