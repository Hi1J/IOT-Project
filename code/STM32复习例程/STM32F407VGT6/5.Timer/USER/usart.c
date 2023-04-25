#include "usart.h"

/*
函数功能：usart初始化
返回值：无
形参：flck , bood, over8
备注：PA9 -- RX   PA10 -- TX
*/
void USART_Init(uint32_t fclk, uint32_t bood, uint8_t over8)
{
	float USARTDIV;    
	uint32_t  DIV_Mantissa;   //整数
  uint32_t  DIV_Fraction;   //小数
	  /* GPIO时钟使能 */
	 RCC->AHB1ENR |= 1 << 0;    //GPIOAs时钟使能
	
	  /* PA9 -- RX*/
	 GPIOA->MODER &= ~(3 << (2 * 9));
	 GPIOA->MODER |= 2 << (2 * 9);  //复用模式
	 
	 GPIOA->AFR[1] &= ~(15 << (4 * (9 - 8)));
	 GPIOA->AFR[1] |= 7 << (4 * (9-8));  //复用为usart1
	 
	 /* PA10 -- TX*/
	 GPIOA->MODER &= ~(3 << (2 * 10));
	 GPIOA->MODER |= 2 << (2 * 10);  //复用模式
	 
	 GPIOA->AFR[1] &= ~(15 << (4 * (10-8)));
	 GPIOA->AFR[1] |= 7 << (4 * (10-8));  //复用为usart1
	 
	 /* 使能串口时钟 */
	RCC->APB2ENR |= 1 << 4;   
	
	/* 设置串口工作方式 */
	USART1->CR1 &= ~(1 << 12);   //数据位8位
	USART1->CR2 &= ~(3 << 12);   //1位停止位
	USART1->CR1 |= 1 << 3;       //发送使能
	USART1->CR1 |= 1 << 2;       //接收使能
	
	/* 配置波特率 */
	if(over8 == 0)
	{
		USART1->CR1 &= ~(1 << 15);   //16倍过采样
	}
	else
	{
		USART1->CR1 |= 1 << 15;     //8倍过采样
	}
	
	 USARTDIV = fclk * 1000000 / (8 * (2 - over8) * bood);
	 DIV_Mantissa = USARTDIV;   //整数
	 DIV_Fraction = (USARTDIV - DIV_Mantissa) * 16; 
	 
	 USART1->BRR = DIV_Mantissa << 4 | DIV_Fraction;
	
	 /* 使能USART*/
	USART1->CR1 |= 1 << 13;  
}



