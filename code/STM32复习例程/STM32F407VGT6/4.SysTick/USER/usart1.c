#include "usart1.h"


void Usart1_Init(uint32_t Baud,uint32_t fclk,uint8_t OVER8)
{
	float USARTDIV;//保存写入BRR寄存器数据
	uint32_t DIV_Mantissa;//保存写入BRR寄存器数据的整数部分
	uint32_t DIV_Fraction;//保存写入BRR寄存器数据的小数部分
	uint32_t Priority;//中断优先级
//开GPIOA时钟
	RCC->AHB1ENR |= 1 << 0;
	
//PA9配置(RX)
	GPIOA->MODER &= ~(3 << (2 * 9));
	GPIOA->MODER |= 2 << (2 * 9);
	
	//配置为USART1复用
	GPIOA->AFR[1] &= ~(15 << ( 4 * (9 - 8)));//清零
	GPIOA->AFR[1] |= 7 << ( 4 * (9 - 8));//写入0111
	
//PA10配置(TX)
	GPIOA->MODER &= ~(3 << (2 * 10));
	GPIOA->MODER |= 2 << (2 * 10);
	
	//配置为USART1复用
	GPIOA->AFR[1] &= ~(15 << ( 4 * (10 - 8)));//清零
	GPIOA->AFR[1] |= 7 << ( 4 * (10 - 8));//写入0111	
	
	
//开USART1时钟
	RCC->APB2ENR |= 1 << 4;
	
//配置串口工作模式CR1
	//字长 0：1位起始位 8位数据位 n位停止位  字长 0：1位起始位 9位数据位 n位停止位
	USART1->CR1 &= ~(1 << 12);
	
	//写入停止位个数 00：1位停止位
	USART1->CR2 &= ~(3 << 12);
	
	//使能发送
	USART1->CR1 |= 1 << 3;
	
	//使能接受
	USART1->CR1 |= 1 << 2;
	
//配置波特率
	//过采样模式的配置 0：16倍过采样 1：8倍过采样
	if(OVER8 == 0)
	{
		USART1->CR1 &= ~(1 << 15);//16倍过采样模式
	}
	else
	{
		USART1->CR1 |= 1 << 15;  //8陪过采样模式
		
	}
	
	
	USARTDIV = fclk * 1000000 /(Baud * 8 * (2 - OVER8));//写入BRR寄存器数据的计算
	
	DIV_Mantissa = USARTDIV;//整数部分
	DIV_Fraction = (USARTDIV - DIV_Mantissa) * 16; // 小数部     * 16 ：也就是左移4位
	
	USART1->BRR = DIV_Mantissa << 4 | DIV_Fraction;//写入BRR寄存器 相当于整数左移4位后+小数
	
//中断初始化
	//使能usart1的接收中断
	USART1->CR1 |= 1 << 5;
	
	NVIC_SetPriorityGrouping(7-2);
	Priority = NVIC_EncodePriority(7-2,1,2);
	NVIC_SetPriority(USART1_IRQn, Priority);
	NVIC_EnableIRQ(USART1_IRQn);
	
	
//使能USART1
	USART1->CR1 |= 1 << 13;
	

}

//发送字符串函数

void Usart1_Send_String(uint8_t * str)
{
	while( *str != '\0')
	{
		USART1->DR = *str;
		while(!(USART1->SR & (1 << 7)));//等待发送完成
		str++;
	}

}

//重写printf函数

int fputc(int ch,FILE *p)
{
	USART1->DR = ch;
	while(!(USART1->SR & (1 << 7 )));
	return ch;


}

//USART1中断服务函数
u8 Rec;
void USART1_IRQHandler(void)
{
	if(USART1->SR & (1 << 5))
	{
		Rec = USART1->DR;
		USART1->SR &= ~(1 << 5); //清空标志位
	
	}


}
