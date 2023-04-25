#include "adc.h"


void ADC1_CH4_Init()
{
	/*配置GPIOA*/
	RCC->AHB1ENR |= 1 << 0;//开启GPIOA时钟
	
	GPIOA->MODER &= ~(3 << (2 * 4));//清零
	GPIOA->MODER |= 3 << (2 * 4);//模拟模式
	
	/*配置ADC1*/
	RCC->APB2ENR |= 1 << 8;//开启ADC1时钟
	
	ADC1->CR1 &= ~(3 << 24);//12位分辨率
	
	ADC1->CR2 &= ~(1 << 11);//右对齐
	
	ADC1->CR2 |= 1 << 10;//使能规则EOC标志位
	
	ADC1->CR2 &= ~(1 << 1);//单次转换模式
	
	ADC->CCR &= ~(3 << 16);//清零
	ADC->CCR |= 1 << 16;//4分频
	
	ADC1->SMPR2 &= ~(7 << (4 * 3));//清零
	ADC1->SMPR2 |= 7 << (4 * 3);//480个周期
	
	
	ADC1->SQR1 &= ~(15 << (4 * 4));//1次转换
	
	
	ADC1->SQR3 |= 4 << (0 * 4);//一次转换
	
	
	ADC1->CR2 |= 1 << 0;   //使能ADC1


}

u16 Get_ADC1_CH4(void)
{
	u16 data;
	
	ADC1->CR2 |= 1 << 30;//展开转换开关
	
	while(!(ADC1->SR & (1 << 1)));//等待转换完成
	
	ADC1->SR &= ~(1 << 1); //标志位清零
	
	data = ADC1->DR;  //读取数据
	
	
	return data;

}


