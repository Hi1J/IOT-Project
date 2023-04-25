#include "led.h"





void LED_Init(void)
{
	//开时钟
 RCC->AHB1ENR |= 1<<2;//GPIOC
	
	//配置GPIO
	 //配置模式
	GPIOC->MODER &= ~(3 << (2*4));//清零
	GPIOC->MODER |= 1 << (2*4); //设置为通用模式
	 //配置输出类型
	GPIOC->OTYPER &=~(1 << 4);//推挽输出
	 //输出速度
	GPIOC->OSPEEDR &=~(3<<(2*4));//清零
	GPIOC->OSPEEDR |=2<<(2*4);//50HMz
	 //设置上下拉
	GPIOC->PUPDR &=~(3<<(2*4));//无上下拉
	
	//设置初始电平为高电平(LED灭)
	GPIOC->ODR |= 1<< 4;
	
	
	
	
	GPIOC->MODER &= ~(3<<(2*5));
	GPIOC->MODER |= 1<<(2*5);
	GPIOC->OTYPER &=~(1<<5);
	GPIOC->OSPEEDR &= ~(3<<(2*5));
	GPIOC->OSPEEDR |= 2<<(2*5);
	GPIOC->PUPDR &= ~(3<<(2*5));
	GPIOC->ODR |= 1<<5;
	
	GPIOC->MODER &= ~(3<<(2*6));
	GPIOC->MODER |= 1<<(2*6);
	GPIOC->OTYPER &=~(1<<6);
	GPIOC->OSPEEDR &= ~(3<<(2*6));
	GPIOC->OSPEEDR |= 2<<(2*6);
	GPIOC->PUPDR &= ~(3<<(2*6));
	GPIOC->ODR |= 1<<6;
	
	
	GPIOC->MODER &= ~(3<<(2*7));
	GPIOC->MODER |= 1<<(2*7);
	GPIOC->OTYPER &=~(1<<7);
	GPIOC->OSPEEDR &= ~(3<<(2*7));
	GPIOC->OSPEEDR |= 2<<(2*7);
	GPIOC->PUPDR &= ~(3<<(2*7));
	GPIOC->ODR |= 1<<7;
	
	
	
	
	
	
	
	
	
	
	



}